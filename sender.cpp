#include "sender.h"
#include <strings.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Sender::Sender(bool stream, std::string ip, int port)
{
    settings.stream = stream;

    _socket = socket(
        AF_INET, 
        settings.stream ? SOCK_STREAM : SOCK_DGRAM, 
        0);
    if (_socket < 0)
        throw new connection_error("Could not create the socket");

    struct sockaddr_in address;
    struct hostent *ip;

    bzero((char *) &address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    if (bind(_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        throw new connection_error("Could not bind the socket");
    }
}

Sender::~Sender()
{
    if (_socket >= 0)
        close(_socket);
}

bool Sender::Send()
{
    auto buffer = getPacket();
    return send(_socket, &buffer[0], buffer.size(), 0)
}

int main(int, char**) {
    
}
