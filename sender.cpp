#include "sender.h"
#include <iostream>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>
#include "libs/aux_functions.h"
#include <unistd.h>
#include <boost/date_time/posix_time/posix_time.hpp>

Sender::Sender(bool stream, std::string ip_str, int port)
{
    settings.stream = stream;

    //opening the socket file
    _socket = socket(
        AF_INET, 
        settings.stream ? SOCK_STREAM : SOCK_DGRAM, 
        0);
    if (_socket < 0)
        throw new connection_error("Could not create the socket");

    //setting the port and IPv4 protocol
    bzero((char *) &_address, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(port);
    //checking whether ip string is in the right format
    struct in_addr addr;
    if (inet_aton(ip_str.c_str(), &addr) < 0)
        throw new connection_error("Could not convert IP");

    _address.sin_addr.s_addr = inet_addr(ip_str.c_str());
}

Sender::~Sender()
{
    //closing the socket if it was opened
    if (_socket >= 0)
        close(_socket);
}

ssize_t Sender::Send()
{
    //connection is established only if we use TCP
    if (!_connected && settings.stream)
    {
        while(connect(_socket, (sockaddr*) &_address, sizeof(_address)) < 0); //for some reason connect does not block execution for me. 
        _connected = true;
    }
    //sending the packet
    auto buffer = getPacket();
    uint32_t packetLen = buffer.size(); //we don't use size_t in case reciever is compiled as a x32 app
    auto len_sent = send(_socket, &packetLen, sizeof(packetLen), settings.stream ? MSG_CONFIRM : 0);

    if (len_sent < 0)
    {
        throw new connection_error("Could not send data to reciever");
    }
    auto data_sent = send(_socket, buffer.data(), buffer.size(), 0);
    if (data_sent < 0)
    {
        throw new connection_error("Could not send data to reciever");
    }
    if (settings.log)
    {
        uint32_t packetId = *((uint32_t*)buffer.data());
        const boost::posix_time::ptime now = *((boost::posix_time::ptime*)(buffer.data() + sizeof(packetId)));
        std::cout << "Sent: #" << packetId 
            << " #" << boost::posix_time::to_iso_extended_string(now) << " UTC"
            << std::endl;
    }
    return len_sent + data_sent;
}

std::vector<uint8_t> Sender::getPacket()
{
    //getting the data to be set (defined in derived classes)
    auto data = getData();
    boost::posix_time::ptime now;
    std::vector<uint8_t> packet(data.size() + 16 + sizeof(_packet_id) + sizeof(now)); //4 bytes for packet id, 16 bytes for md5
    //setting the writing pointer to the packet beginning
    auto ptr = packet.data();
    //writing packet id
    *((uint32_t*)(ptr)) = _packet_id++;
    ptr += sizeof(_packet_id);
    now = boost::posix_time::microsec_clock::universal_time();
    memcpy(ptr, &now, sizeof(now));
    ptr += sizeof(now);
    //copying the data
    memcpy (ptr, data.data(), data.size());
    ptr += data.size();
    //calculating checksum
    md5(&packet[0], sizeof(_packet_id) + data.size() + sizeof(now), ptr);
    return packet;
}

RandomSender::RandomSender(uint32_t lenFrom, uint32_t lenTo, bool stream, std::string ip_str, int port) :
    Sender(stream, ip_str, port),
    _lenFrom(lenFrom),
    _lenTo(lenTo)
{
    srandom(time(nullptr));
    if (_lenFrom > _lenTo)
        std::swap(_lenFrom, _lenTo); //keep the order of limits ascending
}

std::vector<uint8_t> RandomSender::getData()
{
    int len = _lenTo > _lenFrom ? random() % (_lenTo - _lenFrom) + _lenFrom : _lenFrom; //if length are equal no need to randomize
    std::vector<uint8_t> data(len*2);
    for (int i = 0; i < len; ++i)
    {
        long rand = random();
        data[i*2] = static_cast<uint8_t>(rand);
        data[i*2 + 1] = static_cast<uint8_t>(rand >> 8);
    }
    return data;
}

int main(int, char**) {
    try
    {
        RandomSender sender(600, 1600, true, "127.0.0.1", 1992);
        for (int i = 0; i < 1000; ++i)
        {
            sender.Send();
            usleep(10*1000);
        }
        usleep(10*1000*1000);
        for (int i = 0; i < 1000; ++i)
        {
            sender.Send();
            usleep(10*1000);
        }
    }
    catch(std::exception& exc)
    {
        std::cerr << exc.what() << std::endl;
        return -1;
    }
    return 0;
}
