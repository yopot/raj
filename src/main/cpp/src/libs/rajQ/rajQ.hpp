#ifndef RAJ_QUEUE_RAJ
#define RAJ_QUEUE_RAJ

#include <mutex>
#include <queue>
#include <cstdint>

class rajQ
{
public:
 void push(const uint8_t &value);
 uint8_t pop();
 bool isEmpty();

private:
 std::queue<uint8_t> m_queque;
 mutable std::mutex m_mutex;
};

#endif