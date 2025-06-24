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
#include <unordered_map>
#include <deque>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

//include perso
#include "./Print.h"

//using et namespace:
namespace fs = std::filesystem;

//using namespace std;

namespace fs = std::filesystem;

// define de va global

#define CHUNK_SIZE 50


// def chemin;
extern std::string textures_file;
extern std::string worlds_file;
extern std::string world_file;
extern std::string police_source;


#endif
