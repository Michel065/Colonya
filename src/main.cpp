#include "main.h"   

using namespace std;

int main() {
    TimeManager timeManager(10);

    
    
    return 0;
}

#include "TimeManager.h"
#include <iostream>

int main() {
    TimeManager timeManager(10); // 10 ticks par seconde

    // Démarrer la gestion du temps
    std::thread timeThread([&timeManager]() {
        timeManager.start();
    });

    // Simulation principale, tu peux ajouter ici d'autres threads pour les créatures, la carte, etc.
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Changer la vitesse de la simulation
    timeManager.setTimeSpeed(30);  // Passer à 30 ticks par seconde pour accélérer

    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stopper la simulation
    timeManager.stop();

    timeThread.join();  // Attendre que le thread de gestion du temps se termine

    return 0;
}
