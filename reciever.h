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
    //Setters and getters

    void SetLog(bool value)
    {
        settings.log = value;
    }

    bool GetLog() const
    {
        return settings.log;
    }

    void SetVerbose(bool value)
    {
        settings.verbose = value;
    }

    bool GetVerbose() const
    {
        return settings.log && settings.verbose;
    }

    bool GetStream() const
    {
        return settings.stream;
    }
    //Start the threads
    void Run();
private:
    int _socket = -1; //socket file handle
    int _finalSocket = -1; //socket file handle
    sockaddr_in _address; //address of the reciever
    struct sockaddr_in _senderAddr; //address of sender
    socklen_t _senderAddr_len;

    pthread_t _socketThread = -1; //thread that recieves the packets and puts them into 
    pthread_t _processThread = -1; //thread that processes packets
    uint32_t _processDelay; //processing delay
    support::CircularBuffer<std::vector<uint8_t>> _buffer; //circular buffer 

    bool _error = false;
    static void* socketRead(void*);
    static void* process(void*);
    union
    {
        struct
        {
            bool log : 1; //log into the console
            bool stream : 1; //use tcp. false - use udp
            bool verbose : 1; //verbose output
            uint8_t _unused : 5;
        } settings { true, true, true, 0 };
        uint8_t _data;
    };
};

