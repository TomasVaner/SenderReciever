#pragma once
#include <string>
#include <istream>
#include <memory>
#include <vector>
#include <tuple>
#include <openssl/md5.h>

class Sender
{
public:
/**
 * @brief Sends data to the socket
 * 
 * @return true - data was sent
 * @return false - there was an error during sending
 */
    virtual bool Send();
    virtual std::vector<short> GetData();
private:
    unsigned _packet_id;
    
    enum class Protocol : uint8_t
    {
        UDP = 0,
        TCP = 1
    };
    union
    {
        struct
        {
            bool log : 1;
            Protocol protocol : 2;
            uint8_t _unused : 5;
        } settings {true, Protocol::UDP, 0};
        uint8_t _data;
    };

};

class RandomSender : Sender
{
public:
    RandomSender();
private:

};