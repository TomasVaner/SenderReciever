#include "reciever.h"
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>
#include "libs/aux_functions.h"
#include <unistd.h>
#include <system_error>
#include <boost/date_time/posix_time/posix_time.hpp>

Reciever::Reciever(bool stream, std::string ip_str, int port, uint32_t processDelay, size_t bufferCapacity) :
    _buffer(bufferCapacity),
    _processDelay(processDelay)
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
    
    if(bind(_socket, (sockaddr*) &_address, sizeof(_address)) < 0)
    {
        throw new connection_error("Could not bind the socket");
    }
}

Reciever::~Reciever()
{
    //closing the socket if it was opened
    if (_socket >= 0)
        close(_socket);
    if (_finalSocket >= 0)
        close(_finalSocket);
}

void Reciever::Run()
{
    Reciever::socketRead(this); //DEBUG
    if (pthread_create(&_socketThread, NULL, Reciever::socketRead, this))
        throw new std::system_error();
    if (pthread_create(&_processThread, NULL, Reciever::process, this))
    {
        pthread_cancel(_socketThread);
        throw new std::system_error();
    }
    void** ret;
    pthread_join(_socketThread, ret);
    pthread_join(_processThread, ret);
}

void* Reciever::socketRead(void* obj_void)
{
    const auto obj = (Reciever*)obj_void;
    if (obj->settings.stream)
    {
        if (listen(obj->_socket, 5) < 0)
            throw new connection_error("Listen failed");
        if ((obj->_finalSocket = accept(obj->_socket, &obj->_senderAddr, &obj->_senderAddr_len)) < 0)
            throw new connection_error("Count not connect to the sender");
    }
    else
    {
        obj->_finalSocket = obj->_socket;
        obj->_socket = -1;
    }
    std::vector<uint8_t> buffer(obj->_interBuffer_len);
    auto buffer_ptr = buffer.data();
    while(true)
    {
        auto bytes_recv = recv(obj->_finalSocket, buffer_ptr, obj->_interBuffer_len, 0);
        if (bytes_recv != -1)
        {
            std::vector<uint8_t> push_vector(buffer.data(), buffer.data() + bytes_recv);

            if (obj->settings.log)
            {
                uint32_t packetId = *((uint32_t*)buffer.data());
                const boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
                std::cout << "Processed: #" << packetId 
                    << " #" << boost::posix_time::to_iso_extended_string(now)
                    << std::endl;
            }
        }
    }
}

void* Reciever::process(void* obj_void)
{
    auto obj = (Reciever*)obj_void;
    while (true)
    {
        if (!obj->_buffer.IsEmpty())
        {
            auto packet = obj->_buffer.Pop();
            if (packet.has_value())
            {
                uint8_t* ptr = (uint8_t*)packet->data();

                uint8_t hash[16];
                md5(ptr, packet->size() - 16, hash);
                bool pass = memcmp(hash, ptr + packet->size() - 16, 16) == 0;

                if (obj->settings.log)
                {
                    uint32_t packetId = *((uint32_t*)ptr);
                    const boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
                    std::cout << "Processed: #" << packetId 
                        << " #" << boost::posix_time::to_iso_extended_string(now) 
                        << (pass ? " PASS" : " FAIL")
                        << std::endl;
                }
                usleep(obj->_processDelay);
            }
        }
    }
}


int main(int, char**) {
    try
    {
        Reciever reciever(true, "127.0.0.1", 1992, 15, 16);
        reciever.Run();
    }
    catch(std::exception& exc)
    {
        std::cerr << exc.what() << std::endl;
        return -1;
    }
    return 0;
}
