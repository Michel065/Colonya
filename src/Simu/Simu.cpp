#include "Simu.h"

Simulation* Simulation::instance = nullptr;

void start_Time_Manager(TimeManager& tm) {
    print_secondaire("Création d'un tread Time_Manager");
    std::thread timeThread([&tm]() {
        tm.start();
    });
    timeThread.detach();
}

void start_Map_Manager(MapManager& mm) {
    print_secondaire("Création d'un tread Map_Manager");
    std::thread Map_Manaher_Thread([&mm]() {
        mm.start();
    });
    Map_Manaher_Thread.detach();
}

Simulation::Simulation(std::string name, NoiseParam* param_generator)
    : name(name), param_generator(param_generator) {
    print_secondaire("Simulation Créer (", name, ")");
}

Simulation::~Simulation() {
    if (simu_state == SimulationState::Running) {
        stop();
    }
    print_secondaire("Simulation Supprimée (", name, ")");
    if (time_manager) delete time_manager;
    if (map_manager) delete map_manager;
    carte = nullptr;
}

void Simulation::create(std::string name, NoiseParam* param_generator) {
    if (!instance)
        instance = new Simulation(name, param_generator);
    else
        print_error("Simulation déjà créée !");
}

Simulation* Simulation::get_instance() {
    print_secondaire("Simulation::get_instance appelé");
    if(instance){
        return instance;
    }else{
        print_error("Instance non initialisé (Récuperation intance simulation)");
        return nullptr;
    }
}

void Simulation::destroy() {
    if(instance){
        delete instance;
    }
    instance = nullptr;
}

bool Simulation::start() {
    print_primaire("Lancement Simulation (Monde:", name, ").");
    simu_state = SimulationState::Starting;

    time_manager = new TimeManager();
    map_manager = new MapManager(name, *time_manager, param_generator);
    start_Time_Manager(*time_manager);
    start_Map_Manager(*map_manager);

    sf::sleep(sf::seconds(2));
    carte = map_manager->get_map();

    if (carte != nullptr) {
        print_secondaire("Simulation::start() carte chargée");
        simu_state = SimulationState::Running;
        return true;
    }

    print_secondaire_attention("Simulation::start() carte non chargée");
    return false;
}

bool Simulation::stop() {
    simu_state = SimulationState::Closed;
    if (time_manager) time_manager->stop();

    sf::sleep(sf::seconds(0.5));
    print_primaire("Stop de la Simulation (Monde:", name, ").");

    return time_manager ? !time_manager->status() : true;
}

SimulationState Simulation::get_state() {
    return simu_state;
}

std::string Simulation::get_name() {
    return name;
}

void Simulation::set_time_speed(int timespeed) {
    if (time_manager) time_manager->setTimeSpeed(timespeed);
}

void Simulation::pause() {
    if (time_manager) time_manager->pause();
}

void Simulation::reprise() {
    if (time_manager) time_manager->reprise();
}

Map* Simulation::get_carte() {
    return carte;
}

TimeManager* Simulation::get_time_manager() {
    return time_manager;
}

MapManager* Simulation::get_map_manager() {
    return map_manager;
}
