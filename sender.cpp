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
    //connection is established only if we use TCP
    if (settings.stream)
    {
        if(connect(_socket, (sockaddr*) &_address, sizeof(_address)) < 0)
        {
            throw new connection_error("Could not connect to the reciever");
        }
    }
}

Sender::~Sender()
{
    //closing the socket if it was opened
    if (_socket >= 0)
        close(_socket);
}

bool Sender::Send()
{
    //sending the packet
    auto buffer = getPacket();
    bool sent = send(_socket, buffer.data(), buffer.size(), 0) != -1;
    if (sent && settings.log)
    {
        uint32_t packetId = *((uint32_t*)buffer.data());
        const boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        std::cout << "Processed: #" << packetId 
            << " #" << boost::posix_time::to_iso_extended_string(now)
            << std::endl;
    }
    return sent;
}

std::vector<uint8_t> Sender::getPacket()
{
    //getting the data to be set (defined in derived classes)
    auto data = getData();
    std::vector<uint8_t> packet(data.size() + 16 + sizeof(_packet_id)); //4 bytes for packet id, 16 bytes for md5
    //setting the writing pointer to the packet beginning
    auto ptr = packet.data();
    //writing packet id
    *((uint32_t*)(ptr)) = _packet_id++;
    ptr += sizeof(_packet_id);
    //copying the data
    memcpy (ptr, data.data(), data.size());
    ptr += data.size();
    //calculating checksum
    md5(&packet[0], sizeof(_packet_id) + data.size(), ptr);
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
    
}
