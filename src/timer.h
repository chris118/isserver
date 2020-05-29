#ifndef TIMER
#define TIMER

#include <iostream>
#include <thread>
#include <chrono>

namespace hhit
{
class Timer {
    bool clear = false;
    public:
        void setTimeout(std::function<void()> func, int delay);
        void setInterval(std::function<void()> func, int interval);
        void stop();
};
}
#endif