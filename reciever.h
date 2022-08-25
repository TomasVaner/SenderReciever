#pragma once

#include "libs/circular_buffer.h"
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <netinet/in.h>
#include "errors.h"
#include <pthread.h>

class Reciever
{
public:
/**
 * @brief Construct a new Reciever object
 * 
 * @param stream - wether to use stream socket rather than datagram socket
 * @param ip_str - ip of the machine
 * @param port - port to listen/accept
 * @param processDelay - delay of the processing (in ms)
 */
    Reciever(bool stream, std::string ip_str, int port, uint32_t processDelay = 15, size_t bufferCapacity = 16);
    ~Reciever();
    //Start the threads
    void Run();
    void SetInterBufferLen(uint32_t value)
    {
        _interBuffer_len = value;
    }
private:
    int _socket = -1; //socket file handle
    int _finalSocket = -1; //socket file handle
    sockaddr_in _address; //address of the reciever
    struct sockaddr _senderAddr; //address of sender
    socklen_t _senderAddr_len;

    pthread_t _socketThread = -1; //thread that recieves the packets and puts them into 
    pthread_t _processThread = -1; //thread that processes packets
    uint32_t _processDelay; //processing delay
    support::CircularBuffer<std::vector<uint16_t>> _buffer; //circular buffer 
    uint32_t _interBuffer_len = 4096;

    static void* socketRead(void*);
    static void* process(void*);
    union
    {
        struct
        {
            bool log : 1; //log into the console
            bool stream : 1; //use tcp. false - use udp
            uint8_t _unused : 6;
        } settings { true, true, 0 };
        uint8_t _data;
    };
};

