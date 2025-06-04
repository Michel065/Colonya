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

void test_map_chunk(){
    TimeManager time_manager(2);
    start_Time_Manager(time_manager);

    MapManager map_manager("001", time_manager);
    BiomeManager::load_biomes();

    // Création d'une case de test
    Biome* b1 = BiomeManager::get("terre");
    Ressource re;
    re.name = 1;
    Structure st;
    st.name = 42;
    Case c1;
    c1.set_all(b1, &re, &st);

    print(re);

    print(c1.ressource->name);

    Chunk chunk(0,0);
    for (int y = 0; y < 50; ++y){           
        chunk.set_case(0, y, c1);
        print("c1 chunk:");
        print(chunk.at(0,y).ressource->name);
    }
    /*
    // Création d'un Chunk avec cette case partout
    Chunk chunk(0,0);
    for (int x = 0; x < 50; ++x)
        for (int y = 0; y < 50; ++y)
            chunk.set_case(x, y, c1);  // Méthode à créer si pas encore faite

    // Création manuelle du fichier JSON si inexistant
    Map& map = map_manager.get_map();
    
    map.create_json_chunk(chunk);

    // Chargement et sauvegarde pour test
    map.load_chunk(0, 0);
    map.save_chunk(0, 0);
    */
    time_manager.stop();
    std::cout << "Simulation terminée." << std::endl;
}


int main() {
    test_map_chunk();

    return 0;
}
