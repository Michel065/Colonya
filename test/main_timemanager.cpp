#include "main.h"   

using namespace std;

void creatureThread(TimeManager* tm) {
    while (tm->status()) {
        tm->waitNextTick();
        std::cout << "Créature agit !" << std::endl;
    }
}


int main() {
    TimeManager tm(2);

    std::thread t1(creatureThread, &tm);
    std::thread t2(creatureThread, &tm);
    std::thread t3(creatureThread, &tm);

    tm.start();  // Lance la boucle de ticks

    t1.join();
    t2.join();
    t3.join();

    
    
    return 0;
}   