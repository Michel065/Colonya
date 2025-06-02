#include "../Biome.h"
#include "../BiomeManager.h"

void to_json(json& j, const Biome* b) {
    if (b) {
        j = json{{"biome", b->name}};
    } else {
        j = json{{"biome", "unknown"}};
    }
}

void from_json(const json& j, Biome*& b) {
    std::string name;
    j.at("biome").get_to(name);
    b = BiomeManager::get(name);
}