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
    Ressource* re = new Ressource;
    Structure* st = new Structure;
    re->name = 1;
    st->name = 42;
    Case c1;
    c1.set_all(b1, re, st);

    // Création d'un Chunk avec cette case partout
    Chunk chunk(0,0);
    for (int x = 0; x < 50; ++x)
        for (int y = 0; y < 50; ++y)
            chunk.set_case(x, y, c1.clone());  // Méthode à créer si pas encore faite

    // Création manuelle du fichier JSON si inexistant
    Map& map = map_manager.get_map();
    
    map.create_json_chunk(chunk);

    map.load_chunk(0,0);

    map.save_chunk(0,0);

    time_manager.stop();
    std::cout << "Simulation terminée." << std::endl;
}

void test_map_generator(){
    TimeManager time_manager(2);
    start_Time_Manager(time_manager);
    MapManager map_manager("001", time_manager);
    Map& map = map_manager.get_map();

    BiomeManager::load_biomes();

    MapGenerator mapgenerator(42);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            map.create_json_chunk(*mapgenerator.generate_chunk(x,y));
        }
    }  
    

    time_manager.stop();
    std::cout << "Simulation terminée." << std::endl;
}

void test_noise_visualisation() {
    const int nb_cases = 50;
    const int case_size = 10;
    const int size_px = nb_cases * case_size;

    sf::RenderWindow window(sf::VideoMode(size_px, size_px), "Carte Biomes Alt/Hum");

    BiomeManager::load_biomes();
    NoiseGenerator generator(42, 4, 0.5f);

    sf::Image image;
    image.create(size_px, size_px);

    std::map<std::string, sf::Color> biome_colors = {
        {"eau", sf::Color(0, 0, 255)},             // bleu
        {"sable", sf::Color(240, 240, 100)},       // jaune clair
        {"plaine", sf::Color(180, 255, 100)},      // vert clair
        {"herbe", sf::Color(0, 150, 0)},           // vert
        {"roche", sf::Color(80, 80, 80)},          // gris
        {"montagne", sf::Color(0, 0, 0)},          // noir
        {"defaut", sf::Color(255, 0, 255)}         // rose fluo (erreur)
    };

    for (int x = 0; x < nb_cases; ++x) {
        for (int y = 0; y < nb_cases; ++y) {
            float alt = generator.altitude(x, y, 0.05f);
            float hum = generator.humidity(x, y, 0.05f);
            alt = (alt + 1.0f) / 2.0f;
            hum = (hum + 1.0f) / 2.0f;

            Biome* b = BiomeManager::get_best_biome(alt, hum);
            sf::Color color = biome_colors.count(b->name) ? biome_colors[b->name] : biome_colors["defaut"];

            for (int dx = 0; dx < case_size; ++dx)
                for (int dy = 0; dy < case_size; ++dy)
                    image.setPixel(x * case_size + dx, y * case_size + dy, color);
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    window.clear();
    window.draw(sprite);
    window.display();

    sf::sleep(sf::seconds(10));
}



int main() {
    test_map_generator();
    return 0;
}
