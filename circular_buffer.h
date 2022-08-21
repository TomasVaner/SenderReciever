#pragma once
#include <deque>
#include <mutex>

namespace support
{
    template <typename T> class CircularBuffer
    {
    public:
    CircularBuffer (size_t capacity);
    /**
     * @brief Add an item to the buffer
     * 
     * @param item - added item
     * @return true - there was a free space in the buffer to add the item
     * @return false - item was removed to add the new item
     */
    bool Push(T item);
    /**
     * @brief Pops back 1 item from the buffer
     * 
     * @return std::optional<T> - returns nullopt if the buffer is empty
     */
    std::optional<T> Pop();
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

    bool IsEmpty() const;
    bool IsFull() const;

    private:
    std::deque<T> _buffer_inner;
    size_t _capacity;
    }
}