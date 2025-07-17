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
    static Simulation* instance;  // 🔹 l'instance unique

    std::string name;
    NoiseParam* param_generator = nullptr;

    SimulationState simu_state = SimulationState::Closed;

    Map* carte = nullptr;
    MapManager* map_manager = nullptr;
    TimeManager* time_manager = nullptr;

    // 🔒 Constructeur privé interdit les new externes
    Simulation(std::string name, NoiseParam* param_generator = nullptr);

public:
    ~Simulation();

    // 🔹 Création/accès unique à l'instance
    static void create(std::string name, NoiseParam* param_generator = nullptr);
    static Simulation* get_instance();
    static void destroy();

    // ⏯️ Méthodes normales
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
