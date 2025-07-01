#ifndef _TIME_MANAGER_H
#define _TIME_MANAGER_H 

#include "../Commun/includes.h"

class TimeManager {
    
private:
    // 1 tick c 0.5 seconde reel donc le temps reel correspond a 2 tick par seconde
    bool running;
    int ticksPerSecond;
    unsigned long date=0;
    unsigned long duree_simulation=100;
    std::chrono::milliseconds tickDuration;
    std::chrono::steady_clock::time_point lastTickTime;
    std::mutex mtx,pause_mutex;
    std::condition_variable tickCV,eventCV,pause_cv ;
    bool paused = false;



public:
    TimeManager(int tps=2) : running(true),ticksPerSecond(tps) {
        /*
        Cette class a pour role de creer un temps coherent entre nos thread et donc de les syncronisé on fais ca sous forme de tick
        de cette maniere l'idée est de faire en sorte que la vitesse de notre simulation soit reglable.     
        */
        tickDuration = std::chrono::milliseconds(1000 / ticksPerSecond);
    }
    
    void start() {
        print_primaire("Demarage Time Manager!");
        lastTickTime = std::chrono::steady_clock::now();
        while (running && date < duree_simulation) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTickTime);
            if (elapsedTime >= tickDuration) {
                lastTickTime = now;
                std::unique_lock<std::mutex> lock(pause_mutex);
                pause_cv.wait(lock, [&]() { return !paused || !running; });
                lock.unlock();
                tick();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));  // Dormir si pas encore de tick
            }
        }
        print_primaire("STOP Time Manager!");
    }

    void stop() {
        running = false;
        tickCV.notify_all();
        eventCV.notify_all();
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

    void wait_condition_and_tick(unsigned long tick_interval, std::function<bool()> condition) {
        std::unique_lock<std::mutex> lock(mtx);
        unsigned long start_tick = date;

        eventCV.wait(lock, [&] {
            return !running || (date - start_tick >= tick_interval) || condition();
        });
    }

    void signal_event() {
        std::lock_guard<std::mutex> lock(mtx);
        eventCV.notify_all(); // événement externe
    }

    void pause() {
        std::lock_guard<std::mutex> lock(pause_mutex);
        paused = true;
        print_primaire("Pause Time Manager!");
    }

    void reprise() {
        {
            std::lock_guard<std::mutex> lock(pause_mutex);
            paused = false;
        }
        pause_cv.notify_all();
        print_primaire("Reprise Time Manager!");
    }

};

#endif
