#include "Test.h"

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
*/

void test_noise_visualisation() {
    const int nb_cases = 300;
    const int case_size = 4;
    const int size_px = nb_cases * case_size;

    sf::RenderWindow window(sf::VideoMode(size_px, size_px), "Carte Biomes Alt/Hum");

    NoiseGenerator generator(new NoiseParam);

    sf::Image image;
    image.create(size_px, size_px);

    TerrainManager::print_liste_terrain();

    std::map<std::string, sf::Color> terrain_colors = {
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

            Terrain* b = TerrainManager::get_best_terrain(alt, hum);

            sf::Color color = terrain_colors.count(b->name) ? terrain_colors[b->name] : terrain_colors["defaut"];

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

void test_map_generator_visualisation() {
    print_primaire("test_map_generator_visualisation");
    const int nb_cases = CHUNK_SIZE;
    const int case_size = 10;
    const int size_px = nb_cases * case_size;

    sf::RenderWindow window(sf::VideoMode(size_px, size_px), "Carte Biomes Alt/Hum");

    MapGenerator generator(new NoiseParam,{0,0});

    Chunk* chu=generator.generate_chunk(0,0);

    sf::Image image;
    image.create(size_px, size_px);

    TerrainManager::print_liste_terrain();

    std::map<std::string, sf::Color> terrain_colors = {
        {"eau", sf::Color(0, 0, 255)},             // bleu
        {"sable", sf::Color(240, 240, 100)},       // jaune clair
        {"plaine", sf::Color(180, 255, 100)},      // vert clair
        {"herbe", sf::Color(0, 150, 0)},           // vert
        {"roche", sf::Color(80, 80, 80)},          // gris
        {"montagne", sf::Color(0, 0, 0)},          // noir
        {"defaut", sf::Color(255, 0, 255)}         // rose fluo (erreur)
    };

    std::string te="";
    for (int x = 0; x < nb_cases; ++x) {
        for (int y = 0; y < nb_cases; ++y) {

            te=chu->at(x,y)->get_terrain()->name;
            sf::Color color = terrain_colors.count(te) ? terrain_colors[te] : terrain_colors["defaut"];
            print_primaire(te);
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

void mini_test_Map_Manager(){
    print_primaire("!!! mini_test_Map_Manager !!!");

    TimeManager time_manager(2);
    start_Time_Manager(time_manager);
    
    MapManager map_manager("mini_test", time_manager);
    start_Map_Manager(map_manager);
    sf::sleep(sf::seconds(2));

    sf::sleep(sf::seconds(1));
    time_manager.stop();
    sf::sleep(sf::seconds(0.4));
    print_primaire("Simulation terminée.");
}

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
    Map& map = *map_manager.get_map();
    map.print_chunks_load();
    print_status(false,"Get Map manager + print");

    print_secondaire("Wait");
    sf::sleep(sf::seconds(2));
    print_status(true,"On demande a load");
    map_manager.demander_load_chunk(50,50);
    map_manager.demander_load_chunk(51,50);
    sf::sleep(sf::seconds(1));
    map.print_chunks_load();
    print_status(false,"On demande a load");

    print_secondaire("Wait");
    sf::sleep(sf::seconds(2));
    print_status(true,"On demande a deload");
    map_manager.demander_deload_chunk(51,50);
    sf::sleep(sf::seconds(0.2));
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
    c.set_ressource(eau);
    Ressource* tmp;
    Entite ent;

    if (eau) {
        for(int i=0;i<20;i++){
            c.update();
            tmp = c.get_ressource();
            if(tmp)tmp->consommer(ent);
            else print_error("Pas de ressource dans la case ressource.");
            c.update();
        }
    } else {
        print_error("ressource inconnue.");
        return;
    }    

    // Sauvegarde JSON
    json j = c;
    std::cout << "JSON de la case avec eau :\n" << j.dump(4) << std::endl;
}

void test_stockage() {
    print_status(true, "test_stockage");

    Stockage stockage(3);
    print_secondaire("→ Création d’un stockage capacité 3");

    Ressource* r1 = RessourceManager::creer(RessourceType::EAU);
    Ressource* r2 = RessourceManager::creer(RessourceType::EAU);
    Ressource* r3 = RessourceManager::creer(RessourceType::EAU);
    Ressource* r4 = RessourceManager::creer(RessourceType::EAU);

    print_secondaire("→ Ajout de 3 ressources (limite atteinte à 3)");
    stockage.ajouter(r1) ? print_primaire("  ✓ Ajout r1") : print_error("  ✗ Échec ajout r1");
    stockage.ajouter(r2) ? print_primaire("  ✓ Ajout r2") : print_error("  ✗ Échec ajout r2");
    stockage.ajouter(r3) ? print_primaire("  ✓ Ajout r3") : print_error("  ✗ Échec ajout r3");

    print_secondaire("→ Tentative d’ajout au-delà de la limite");
    if (!stockage.ajouter(r4)) {
        print_primaire("  ✓ Rejet correct de r4 car stockage plein");
        delete r4;
    } else {
        print_error("  ✗ r4 ajouté alors que stockage est plein !");
    }

    print_secondaire("→ Vérification snapshot() et taille()");
    auto snap = stockage.snapshot();
    print_primaire("  ✓ Taille actuelle : ", snap.size());

    print_secondaire("→ Retrait de la première ressource");
    Ressource* retiré = nullptr;
    if (stockage.retirer(retiré)) {
        print_primaire("  ✓ Retrait réussi : ", retiré->get_name());
        delete retiré;
    } else {
        print_error("  ✗ Retrait échoué");
    }

    print_secondaire("→ Vérification après retrait");
    print_primaire("  ✓ Taille actuelle : ", stockage.taille());

    print_secondaire("→ Test clear()");
    stockage.clear();
    print_primaire("  ✓ Stockage vidé, taille : ", stockage.taille());

    print_secondaire("→ Ajout pour test JSON (x2)");
    stockage.ajouter(RessourceManager::creer(RessourceType::EAU));
    stockage.ajouter(RessourceManager::creer(RessourceType::EAU));

    print_secondaire("→ Sérialisation JSON");
    nlohmann::json j = stockage;
    print_primaire("  ✓ JSON : ", j.dump());

    print_secondaire("→ Désérialisation dans stockage2");
    Stockage stockage2;
    from_json(j, stockage2);

    print_primaire("  ✓ Taille de stockage2 : ", stockage2.taille());

    std::vector<RessourceType> liste = stockage2.get_all_ressource();

    print_secondaire("[");
    for (auto& r : liste) {
        print_secondaire(r,", ");
    }
    print_secondaire("]");

    stockage.clear();
    stockage2.clear();

    print_status(false, "test_stockage");
}

void test_hub() {
    print_status(true, "test_hub");
    print_status(true,"Init map vide");
    Map* map = new Map;
    print_status(false,"Init map vide");

    // Setup
    Hub hub(map);
    Entite* e1 = new Entite("Bob");
    Entite* e2 = new Entite("Alice");

    Ressource* r1 = RessourceManager::creer(RessourceType::EAU);
    Ressource* r2 = RessourceManager::creer(RessourceType::EAU);

    print_secondaire("→ Ajout de ressources dans le Hub");
    hub.ajouter(r1) ? print_primaire("  ✓ Ajout r1") : print_error("  ✗ Ajout r1 échoué");
    hub.ajouter(r2) ? print_primaire("  ✓ Ajout r2") : print_error("  ✗ Ajout r2 échoué");

    print_secondaire("→ Entrée d'entités");
    hub.entrer(e1) ? print_primaire("  ✓ Bob est entré") : print_error("  ✗ Bob a été refusé");
    hub.entrer(e2) ? print_primaire("  ✓ Alice est entrée") : print_error("  ✗ Alice a été refusée");

    print_secondaire("→ Test fonction dormir");
    if (hub.dormir(e1)) print_primaire("  ✓ Bob a dormi");
    else print_error("  ✗ Bob n'a pas pu dormir");

    print_secondaire("→ Vérification entités présentes");
    auto liste = hub.get_liste_des_entite_presente();  // type : std::vector<Entite*>
    for (auto* e : liste) {
        print("   - ", e->get_name());
    }

    print_secondaire("→ Sauvegarde JSON");
    nlohmann::json j = hub.get_json();
    print_primaire("  ✓ JSON : ", j.dump());

    print_secondaire("→ Chargement depuis JSON");
    Hub hub2;
    hub2.from_json(j);

    print_secondaire("→ Résumé hub2 :");
    print(hub2.get_print_string());

    // Nettoyage (manuellement car pas encore smart_ptr)
    delete e1;
    delete e2;

    print_status(false, "test_hub");
}

void test_structure_serialisation() {
    print_status(true, "test_structure_serialisation");

    // Création d’un Hub
    Hub* hub = new Hub();
    hub->ajouter(RessourceManager::creer(RessourceType::EAU));
    Entite* e = new Entite("Jean");
    hub->entrer(e);

    print_secondaire("→ JSON initial du Hub");
    json j = *hub;
    print_primaire("  ✓ JSON : ", j.dump());

    print_secondaire("→ Reconstruction via Structure*");
    Structure* structure = nullptr;
    from_json(j, structure);

    if (structure) {
        print_primaire("  ✓ Structure reconstruite");
        print(structure->get_print_string());
    } else {
        print_error("  ✗ Reconstruction échouée !");
    }

    delete hub;
    delete e;
    delete structure;

    print_status(false, "test_structure_serialisation");
}

void test_display(){
    print_status(true, "test_display");

    DisplayManager fenetre;
    fenetre.run();
    
    print_status(false, "test_display");
}

void test_simulation() {
    print_status(true, "test_simulation");

    // 🔹 Création de la simulation via la méthode statique
    Simulation::create("001");
    Simulation* simu = Simulation::get_instance();

    if (!simu) {
        print_error("Échec de l'initialisation de la simulation.");
        return;
    }

    print_secondaire("Simulation initialisée.");
    bool ok = simu->start();

    if (!ok) {
        print_error("Échec du démarrage de la simulation.");
        Simulation::destroy();
        return;
    }

    print_primaire("Simulation lancée. Attente de l'état 'Running'...");

    SimulationState state = simu->get_state();

    if (state != SimulationState::Running) {
        print_error("État 'Running' non atteint.");
        Simulation::destroy();
        return;
    }

    print_primaire("Simulation prête.");

    // Pause
    print_secondaire("Pause de la simulation...");
    simu->pause();
    sf::sleep(sf::milliseconds(300));
    print_secondaire("Reprise de la simulation...");
    simu->reprise();

    // Changement vitesse
    print_secondaire("Changement vitesse temps à 10 ticks/sec");
    simu->set_time_speed(10);
    sf::sleep(sf::milliseconds(300));

    // Accès aux composants
    if (simu->get_carte()) print_test("Carte disponible.");
    if (simu->get_time_manager()) print_test("TimeManager OK.");
    if (simu->get_map_manager()) print_test("MapManager OK.");

    // Attente quelques ticks
    print_secondaire("Attente 5 ticks...");
    simu->get_time_manager()->wait_ticks(5);
    print_test("Date actuelle : ", simu->get_time_manager()->get_date());

    // Stop
    print_secondaire("Arrêt de la simulation...");
    bool stoppe = simu->stop();

    if (stoppe)
        print_primaire("Simulation arrêtée avec succès.");
    else
        print_error("Erreur lors de l'arrêt de la simulation.");

    // 🔻 Destruction explicite de l'instance unique
    Simulation::destroy();

    print_status(false, "test_simulation");
}

void test_entite() {
    print_status(true, "test_entite");

    Simulation::create("001");
    Simulation* simu = Simulation::get_instance();

    Entite* e = new Entite("TestEntity");
    
    e->set_map_manager(simu->get_map_manager());
    e->set_time_manager(simu->get_time_manager());

    int x = 5;
    int y = 5;
    
    e->set_position(x,y);
    
    print(e->get_name());

    delete e;
    print_status(false, "test_entite");

}


int main_test(){
    print_primaire("!!! MODE TEST !!!");
    test_display();
    print_primaire("!!! FIN MODE TEST !!!");
    return 0;
}