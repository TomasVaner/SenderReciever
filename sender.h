#pragma once
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <netinet/in.h>
#include "errors.h"

class Sender
{
public:
    Sender(bool stream, std::string ip_str, int port);
    virtual ~Sender();

/**
 * @brief Sends data to the socket
 * 
 * @return true - data was sent
 * @return false - there was an error during sending
 */
    ssize_t Send();

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
private:
    uint32_t _packet_id = 0; //id of the packet to be sent (number of the packet)
    int _socket = -1; //socket file handle
    sockaddr_in _address; //address of the reciever
    bool _connected = false;
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
    std::vector<uint8_t> getPacket();
/**
 * @brief Get the data to be sent in the packet
 * 
 * @return std::vector<uint8_t> - data to be sent
 */
    virtual std::vector<uint8_t> getData() = 0;
};

class RandomSender : public Sender
{
public:
    RandomSender(uint32_t lenFrom, uint32_t lenTo, bool stream, std::string ip_str, int port);
private:
    uint32_t _lenFrom, _lenTo; //range of the length of the data to be generated. 16-bit words count

    virtual std::vector<uint8_t> getData() override;
};