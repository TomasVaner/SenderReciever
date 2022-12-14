#pragma once
#include <deque>
#include <mutex>
#include <optional>

namespace support
{
    template <typename T> class CircularBuffer
    {
    public:
        CircularBuffer (size_t capacity) :
            _capacity(capacity)
        {
        }
        /**
         * @brief Add an item to the buffer
         * 
         * @param item - added item
         * @return true - there was a free space in the buffer to add the item
         * @return false - item was removed to add the new item
         */
        bool Push(T item)
        {
            std::scoped_lock lock(_mutex);
            bool ret = true;
            if (_buffer_inner.size() == _capacity)
            {
                _buffer_inner.pop_front();
                ret = false;
            }
            _buffer_inner.push_back(item);
            return ret;
        }
        /**
         * @brief Pops back 1 item from the buffer
         * 
         * @return std::optional<T> - returns nullopt if the buffer is empty
         */
        std::optional<T> Pop()
        {
            std::scoped_lock lock(_mutex);
            if (_buffer_inner.empty())
                return std::nullopt;
            else
            {
                auto ret = _buffer_inner.front();
                _buffer_inner.pop_front();
                return ret;
            }
        }
        /**
         * @brief Return the count of items in the buffer
         */
        size_t Size() const
        {
            return _buffer_inner.size();
        }
        /**
         * @brief Return the capacity of the buffer
         */
        size_t Capacity() const noexcept
        {
            return _capacity;
        }

        bool IsEmpty() const
        {
            return _buffer_inner.empty();
        }

        bool IsFull() const
        {
            return _buffer_inner.size() == _capacity;
        }

    private:
        std::deque<T> _buffer_inner;
        size_t _capacity;
        std::mutex _mutex;
    };    
}