#include "circular_buffer.h"

namespace support
{
    template<typename T>
    CircularBuffer<T>::CircularBuffer(size_t capacity)
        _capacity(capacity)
        , _buffer_inner(capacity)
    {
        
    }

    template<typename T>
    bool CircularBuffer<T>::Push(T item)
    {
        bool ret = true;
        if (_buffer_inner.size() == _capacity)
        {
            _buffer_inner.pop_front();
            ret = false;
        }
        _buffer_inner.push_back();
        return ret;
    }

    template<typename T>
    std::optional<T> CircularBuffer<T>::Pop()
    {
        return _buffer_inner.empty() ? std::nullopt : _buffer_inner.pop_front();
    }
}