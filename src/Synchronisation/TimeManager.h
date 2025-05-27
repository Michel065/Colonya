#ifndef _TIME_MANAGER_H
#define _TIME_MANAGER_H 

#include <condition_variable>
#include <mutex>
#include <atomic>

class TimeManager {
private:
    std::atomic<bool> running;
    int ticksPerSecond;
    std::chrono::milliseconds tickDuration;
    std::chrono::steady_clock::time_point lastTickTime;

    std::mutex mtx;
    std::condition_variable tickCV;

public:
    TimeManager(int tps) : running(true),ticksPerSecond(tps) {
        tickDuration = std::chrono::milliseconds(1000 / ticksPerSecond);
    }

    void start() {
        int x=0;
        lastTickTime = std::chrono::steady_clock::now();
        while (running) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTickTime);
            if (elapsedTime >= tickDuration) {
                lastTickTime = now;
                tick();
                x++;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            if(x>10){stop();}
        }
    }

    void stop() {
        running = false;
        tickCV.notify_all();
    }

    void tick() {
        std::cout << "Tick!" << std::endl;
        tickCV.notify_all();
    }

    void waitNextTick() {
        std::unique_lock<std::mutex> lock(mtx);
        tickCV.wait(lock); // Attend jusqu'à la prochaine notification
    }

    void setTimeSpeed(int newTicksPerSecond) {
        ticksPerSecond = newTicksPerSecond;
        tickDuration = std::chrono::milliseconds(1000 / ticksPerSecond);
    }

    bool status() {
        return running;
    }
};

#endif
