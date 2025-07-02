#ifndef _SIMU_H
#define _SIMU_H 

#include "../Commun/includes.h"
#include "./Synchronisation/TimeManager.h"
#include "./Map/MapManager.h"
#include "SimulationState.h"

void start_Time_Manager(TimeManager &tm);
void start_Map_Manager(MapManager &mm);

class Simulation {
private:
    std::string name;
    NoiseParam* param_generator = nullptr;

    SimulationState simu_state = SimulationState::Closed;

    Map* carte = nullptr;
    MapManager* map_manager = nullptr;
    TimeManager* time_manager = nullptr;

public:
    Simulation(std::string name, NoiseParam* param_generator = nullptr);
    ~Simulation();

    bool start();
    bool stop();

    SimulationState get_state();
    std::string get_name();
    void set_time_speed(int timespeed);
    void pause();
    void reprise();

    Map* get_carte();
    TimeManager* get_time_manager();
    MapManager* get_map_manager();
};

#endif
