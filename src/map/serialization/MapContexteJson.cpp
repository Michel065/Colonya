#include "../MapContexte.h"

void to_json(json& j, const MapContexte& m) {
    j = json{{"chuncks", m.chuncks}};
}

void from_json(const json& j, MapContexte& m) {
    j.at("chuncks").get_to(m.chuncks);
}
