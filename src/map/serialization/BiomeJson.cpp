#include "../Biome.h"

void to_json(json& j, const Biome& m) {
    j = json{{"name_biome", m.name_biome}};
}

void from_json(const json& j, Biome& m) {
    j.at("name_biome").get_to(m.name_biome);
}
