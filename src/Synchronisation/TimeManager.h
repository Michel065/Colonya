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
        /*
        Cette class a pour role de creer un temps coherent entre nos thread et donc de les syncronisé on fais ca sous forme de tick
        de cette maniere l'idée est de faire en sorte que la vitesse de notre simulation soit reglable.     
        */
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
        //if(date > duree_simulation){stop();}  // Arrêter la simulation une fois la durée atteinte
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

    void wait_ticks(unsigned long ticks) {
        wait_until(date + ticks);
    }

    void wait_until(unsigned long target_tick) {
        std::unique_lock<std::mutex> lock(mtx);
        tickCV.wait(lock, [&]() {
            return date >= target_tick || !running;
        });
    }
};

#endif
