#include "rajQ.hpp"

void rajQ::
push(const uint8_t &value)
{
 std::lock_guard<std::mutex> lock(m_mutex);
 m_queque.push(value);
}

uint8_t rajQ::
pop()
{
 std::lock_guard<std::mutex> lock(m_mutex);
 uint8_t r = m_queque.front();
 m_queque.pop();
 return r;
}

bool rajQ::
isEmpty()
{
 std::lock_guard<std::mutex> lock(m_mutex);
 return m_queque.empty();
}