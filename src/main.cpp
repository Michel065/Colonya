#include "main.h"    

void start_Time_Manager(TimeManager &tm) {
    // Lancer le gestionnaire de temps dans un thread séparé
    std::thread timeThread([&tm]() {
        tm.start();
    });

    timeThread.detach();  // Détacher le thread pour qu'il fonctionne en arrière-plan
}

void test_time_manager(){
    TimeManager tm(2);

    start_Time_Manager(tm);

    // Pendant que le thread fonctionne, on peut faire d'autres actions
    for (int i = 0; i < 5; ++i) {
        // Attendre chaque tick pour simuler une autre tâche
        tm.waitNextTick();
        std::cout << "Attente d'un nouveau tick..." << tm.get_date() << std::endl;
    }

    // Arrêter le gestionnaire de temps
    tm.stop();
    
    std::cout << "Simulation terminée." << std::endl;

}

int main() {
    TimeManager time_manager(2);

    start_Time_Manager(time_manager);

    MapManager map_manager("001",time_manager);

    BiomeManager::load_defaults();

    Biome* b1=BiomeManager::get("terre");

    Ressource re;
    re.name=1;
    Structure st;
    st.name=34;
    Case c1;
    c1.set(b1,re,st);


    std::cout<<c1<<std::endl;

    time_manager.stop();
    
    std::cout << "Simulation terminée." << std::endl;

    return 0;
}
