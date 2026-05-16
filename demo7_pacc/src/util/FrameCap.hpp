#pragma once
#include <chrono>
#include <thread>

class FrameCap {
    public:
        using Clock = std::chrono::steady_clock;

        explicit FrameCap(const int fps)
            : interval{Clock::duration{Clock::duration::period::den / fps}},
            start{Clock::now()} {}

        ~FrameCap() {
            std::this_thread::sleep_until(start + interval);
        }

    private:
        const Clock::duration interval;
        const Clock::time_point start;
};

