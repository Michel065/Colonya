#ifndef _TIME_MANAGER_H
#define _TIME_MANAGER_H 

#include "../includes.h"

class TimeManager {
private:
    // 1 tick c 0.5 seconde reel donc le temps reel correspond a 2 tick par seconde
    std::atomic<bool> running;
    int ticksPerSecond;
    unsigned long date=0;
    unsigned long duree_simulation=100;
    std::chrono::milliseconds tickDuration;
    std::chrono::steady_clock::time_point lastTickTime;
    std::mutex mtx;
    std::condition_variable tickCV;


public:
    TimeManager(int tps) : running(true),ticksPerSecond(tps) {
        tickDuration = std::chrono::milliseconds(1000 / ticksPerSecond);
    }

    void start() {
        lastTickTime = std::chrono::steady_clock::now();
        while (running && date < duree_simulation) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTickTime);
            if (elapsedTime >= tickDuration) {
                lastTickTime = now;
                tick();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));  // Dormir si pas encore de tick
            }
        }
        stop();  // Arrêter la simulation une fois la durée atteinte
    }

    void stop() {
        std::cout << "STOP Time Manager!" << std::endl;
        running = false;
        tickCV.notify_all();
    }

    void tick() {
        //std::cout << "Tick!" << std::endl;
        date++;
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

    unsigned long get_date() {
        return date;
    }
};

#endif
