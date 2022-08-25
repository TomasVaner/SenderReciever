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
    bool Send();

//Setters and getters

    void SetLog(bool value)
    {
        settings.log = value;
    }

    bool GetLog() const
    {
        return settings.log;
    }

    bool GetStream() const
    {
        return settings.stream;
    }
/**
 * @brief Checks if the socket is still alive
 * 
 * @return true 
 * @return false 
 */
    bool IsAlive() const;

private:
    uint32_t _packet_id = 0; //id of the packet to be sent (number of the packet)
    int _socket = -1; //socket file handle
    sockaddr_in _address; //address of the reciever
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