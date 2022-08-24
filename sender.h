#pragma once
#include <string>
#include <istream>
#include <memory>
#include <vector>
#include <tuple>
#include <openssl/md5.h>
#include "errors.h"

class Sender
{
public:
    Sender(bool stream, std::string ip, int port);
    ~Sender();

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

    void SetStream(bool value)
    {
        settings.stream = value;
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
    unsigned _packet_id = 0;
    int _socket = -1;
    union
    {
        struct
        {
            bool log : 1;
            bool stream : 1;
            uint8_t _unused : 6;
        } settings { true, true, 0 };
        uint8_t _data;
    };
    std::vector<uint8_t> getPacket();
protected: 
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
    RandomSender(uint32_t lenFrom, uint32_t lenTo, bool stream, std::string ip, int port);
    virtual std::vector<uint8_t> GetData() override;
private:
    uint32_t lenFrom, lenTo;
};