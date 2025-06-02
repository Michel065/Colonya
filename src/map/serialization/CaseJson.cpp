#include "../Case.h"

void to_json(json& j, const Case& c) {
    j = json{
        {"biome", c.biome},
        {"ressource", c.ressource},
        {"structure", c.structure}
    };
}

void from_json(const json& j, Case& c) {
    j.at("biome").get_to(c.biome);
    j.at("ressource").get_to(c.ressource);
    j.at("structure").get_to(c.structure);
}
