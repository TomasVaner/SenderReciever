#pragma once
#include <exception>

class connection_error : public std::exception
{
public:
    connection_error(const char * what = "Connection Error") :
        std::exception(),
        _what(what)
    {

    };
    const char * what () const throw () {
        return _what;
    }
private:
    const char * _what;
};