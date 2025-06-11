#include "Test.h"

void start_Time_Manager(TimeManager &tm) {
    std::thread timeThread([&tm]() {
        tm.start();
    });

    timeThread.detach();
}

void start_Map_Manager(MapManager &mm) {
    std::thread Map_Manaher_Thread([&mm]() {
        mm.start();
    });

    Map_Manaher_Thread.detach();
}
/*
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

    MapGenerator mapgenerator;

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            map.create_json_chunk(*(mapgenerator.generate_chunk(x,y)));
        }
    }  
    

    time_manager.stop();
    std::cout << "Simulation terminée." << std::endl;
}

void test_noise_visualisation() {
    const int nb_cases = 300;
    const int case_size = 3;
    const int size_px = nb_cases * case_size;

    sf::RenderWindow window(sf::VideoMode(size_px, size_px), "Carte Biomes Alt/Hum");

    BiomeManager::load_biomes();
    NoiseGenerator generator(new NoiseParam);

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
            float alt = generator.altitude(x, y);
            float hum = generator.humidity(x, y);
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

    sf::sleep(sf::seconds(100));
}
*/

void test_Map_Manager() {
    print_primaire("Debut Simulation.");
    print_status(true,"Init(Thread)");
    TimeManager time_manager(2);
    start_Time_Manager(time_manager);
    MapManager map_manager("001", time_manager);
    start_Map_Manager(map_manager);
    print_status(false,"Init(Thread)");
    print_secondaire("Wait");
    sf::sleep(sf::seconds(2));

    print_status(true,"Get Map manager + print");
    Map& map = map_manager.get_map();
    map.print_chunks_load();
    print_status(false,"Get Map manager + print");

    print_secondaire("Wait");
    sf::sleep(sf::seconds(2));
    print_status(true,"On demande a load");
    map_manager.demander_load_chunk(50,50);
    map_manager.demander_load_chunk(51,50);
    map.print_chunks_load();
    print_status(false,"On demande a load");

    print_secondaire("Wait");
    sf::sleep(sf::seconds(2));
    print_status(true,"On demande a deload");
    map_manager.demander_deload_chunk(51,50);
    map.print_chunks_load();
    print_status(false,"On demande a deload");

    
    print_secondaire("Wait");
    sf::sleep(sf::seconds(0.1));
    time_manager.stop();
    print_secondaire("Wait");
    sf::sleep(sf::seconds(0.4));
    print_primaire("Simulation terminée.");
}

void test_ressource_manager() {
    // Création et test d'une ressource manuelle
    Ressource* eau = RessourceManager::creer(RessourceType::EAU);

    // Création d'une Case avec une ressource Eau
    Case c;
    c.ressource = eau;

    Entite ent;

    if (eau) {
        for(int i=0;i<19;i++){
            eau->consommer(&ent,c);
        }
        eau->consommer(&ent,c);
    } else {
        print_error("ressource inconnue.");
        return;
    }    

    // Sauvegarde JSON
    json j = c;
    std::cout << "JSON de la case avec eau :\n" << j.dump(4) << std::endl;
}


int main_test(){
    print_primaire("!!! MODE TEST !!!");
    test_ressource_manager();
    print_primaire("!!! FIN MODE TEST !!!");
    return 0;
}