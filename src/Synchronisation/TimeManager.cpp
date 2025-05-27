#include <chrono>
#include <thread>
#include <atomic>

class TimeManager {
private:
    std::atomic<bool> running; // Permet de savoir si la simulation est en cours
    int ticksPerSecond; // Nombre de ticks par seconde
    std::chrono::milliseconds tickDuration; // Durée d'un tick en millisecondes
    std::chrono::steady_clock::time_point lastTickTime; // Moment du dernier tick

public:
    TimeManager(int tps) : ticksPerSecond(tps), running(true) {
        tickDuration = std::chrono::milliseconds(1000 / ticksPerSecond);
    }

    // Démarrer la gestion du temps
    void start() {
        lastTickTime = std::chrono::steady_clock::now();

        while (running) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTickTime);

            if (elapsedTime >= tickDuration) {
                // Un nouveau tick doit être effectué
                lastTickTime = now;
                tick();  // Appeler la méthode de mise à jour des threads
            } else {
                // Si le temps n'est pas écoulé, on attend un peu avant de vérifier à nouveau
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }

    // Stopper la simulation
    void stop() {
        running = false;
    }

    // Implémentation du tick, met à jour tous les threads ici
    void tick() {
        // Cette méthode sera appelée chaque fois qu'un tick se produit
        // Il faut informer les autres threads (créatures, carte, etc.) que le temps est passé

        // Exemple :
        // - Mise à jour de la carte
        // - Mise à jour des entités
        // - Calcul du cycle jour/nuit
        // Tu peux appeler ici des méthodes de chaque thread que tu as déjà créées
        std::cout << "Tick! Mise à jour des entités, carte, ressources..." << std::endl;
    }

    // Permet de changer l'accélération de la simulation
    void setTimeSpeed(int newTicksPerSecond) {
        ticksPerSecond = newTicksPerSecond;
        tickDuration = std::chrono::milliseconds(1000 / ticksPerSecond);
    }
};
