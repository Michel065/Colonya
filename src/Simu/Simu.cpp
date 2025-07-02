#include "Simu.h"

//les starteurs
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





//la simu

Simulation::Simulation(std::string name, NoiseParam* param_generator)
    : name(name), param_generator(param_generator) {}

Simulation::~Simulation() {
    if(simu_state==SimulationState::Running){
        stop();
    }
    if(time_manager)delete time_manager;
    if(map_manager)delete map_manager;
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
        simu_state = SimulationState::Running;
        return true;
    }

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

std::string Simulation::get_name(){
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
