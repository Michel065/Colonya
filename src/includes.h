#ifndef _INCLUDES_H
#define _INCLUDES_H 


//include:
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <vector>
#include <random>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <chrono>
#include <string>
#include <filesystem>
#include <functional>
#include <cmath>
#include <algorithm>
#include <unordered_set>


#include <nlohmann/json.hpp>
using json = nlohmann::json;


#include <unordered_map>

//using et namespace:
namespace fs = std::filesystem;

//using namespace std;


// define de va global

#define CHUNK_SIZE 50
#define PRINT__ true


//fonction de base

template<typename... Args>
inline void print(const Args&... args) {
    if constexpr (PRINT__) {
        (std::cout << ... << args) << std::endl;
    }
}

template<typename... Args>
inline void print_test(const Args&... args) {
    (std::cout << ... << args) << std::endl;
}


template<typename T>
inline void print_status(const T& trucs, bool status=true) {
    if constexpr (!PRINT__) return;
    if(status){
        std::cout << trucs <<" ..." <<std::endl;
    }
    else{
        std::cout << trucs <<" Done!" <<std::endl;
    }
}


// def chemin;
extern std::string textures_file;
extern std::string worlds_file;
extern std::string world_file;


#endif
