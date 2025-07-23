#include "MemoireIAEncoder.h"

MemoireIAEncoder::MemoireIAEncoder(const std::string& source_file, const std::string& name_creature)
    : filepath(source_file + "/" + name_creature + ".json"), source_dir(source_file)
{
    if (!fs::exists(source_dir)) {
        fs::create_directories(source_dir);
    }

    if (fs::exists(filepath)) {
        load_from_json();
    } else {
        save_to_json();
    }
}

MemoireIAEncoder::~MemoireIAEncoder() {
    save_to_json();
}


std::vector<float> MemoireIAEncoder::vecteur_observation_par_defaut() const{
    // [0] walkable (0 ou 1)
    // [1] potentiel faim (0.0 à 1.0)
    // [2] potentiel soif (0.0 à 1.0)
    // [3] potentiel sommeil (0.0 à 1.0)
    // [4] distance normalisée à la créature (0.0 à 1.0)
    // [5] angle relatif à la créature (−1.0 à 1.0)
    return std::vector<float>(TAILLE_VECTOR_OBSERVATION, 0.0f);
}

std::vector<float> MemoireIAEncoder::vecteur_action_par_defaut() const{
    // [0] faim 
    // [1] soif 
    // [2] sommeil 
    // [3] déplacement
    // [4] rotation
    // [5] stockage (générique)
    // [6] transfert_vers_stockage
    // [7] retrait_depuis_stockage
    // [8] manger
    // [9] boire
    // [10] dormir
    // [11] ramasser ressource
    // [12] poser ressource
    // [13] interaction structure
    // [14] vision (déclenche une analyse)
    // [15] danger/évitement
    // [16] exploration/général
    return std::vector<float>(TAILLE_VECTOR_ACTION, 1.0f);
}

std::vector<float> MemoireIAEncoder::encoder_observation(const Observation& obs, const Entite& creature) {
    std::vector<float> features = vecteur_observation_par_defaut();

    features[0] = obs.walkable ? 1.0f : 0.0f;

    auto it = label_effects.find(obs.label);
    if (it != label_effects.end()) {
        for (int i = 0; i < 3 && i <  static_cast<int>(it->second.size()); ++i) {
            features[1 + i] = it->second[i];
        }
    }

    float dx = obs.x - creature.get_position().first;
    float dy = obs.y - creature.get_position().second;

    float dist = std::sqrt(dx * dx + dy * dy);
    features[4] = std::min(dist / 10.f, 1.0f);

    float angle = std::atan2(dy, dx) - creature.get_orientation();
    while (angle > M_PI) angle -= 2 * M_PI;
    while (angle < -M_PI) angle += 2 * M_PI;
    features[5] = angle / M_PI; // normalisé [-1 ; 1]

    return features;
}

std::vector<float> MemoireIAEncoder::encoder_action(const std::string& nom_action) {
    auto it = encodage_action_par_nom.find(nom_action);
    if (it != encodage_action_par_nom.end()) return it->second;

    encodage_action_par_nom[nom_action] = vecteur_action_par_defaut();
    return encodage_action_par_nom[nom_action];
}

EncodedAction MemoireIAEncoder::encoder(const Action& action) {
    return EncodedAction{
        action.nom,
        encoder_action(action.nom)
    };
}

EncodedObservation MemoireIAEncoder::encoder(const Observation& obs, const Entite& creature) {
    EncodedObservation eo;
    eo.features = encoder_observation(obs, creature);
    return eo;
}

void MemoireIAEncoder::update_action_encoding(const std::string& nom, const std::vector<float>& vec) {
    encodage_action_par_nom[nom] = vec;
}

void MemoireIAEncoder::update_label_effect(const std::string& label, const std::vector<float>& vec) {
    label_effects[label] = vec;
}

EncodedObservation MemoireIAEncoder::get_encoded_observation(const std::string& label) const {
    auto it = label_effects.find(label);
    if (it != label_effects.end()) {
        return EncodedObservation{it->second};
    }
    return EncodedObservation{vecteur_observation_par_defaut()};
}

EncodedAction MemoireIAEncoder::get_encoded_action(const std::string& nom) const {
    auto it = encodage_action_par_nom.find(nom);
    if (it != encodage_action_par_nom.end()) {
        return EncodedAction{nom, it->second};
    }
    return EncodedAction{nom, vecteur_action_par_defaut()};
}

const std::unordered_map<std::string, std::vector<float>>& MemoireIAEncoder::get_encodage_action_map() const {
    return encodage_action_par_nom;
}

const std::unordered_map<std::string, std::vector<float>>& MemoireIAEncoder::get_label_effect_map() const {
    return label_effects;
}

void MemoireIAEncoder::save_to_json() const {
    json j;
    j["actions"] = encodage_action_par_nom;
    j["labels"] = label_effects;
    std::ofstream file(filepath);
    if (file.is_open()) file << j.dump();
}

void MemoireIAEncoder::load_from_json() {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    json j;
    file >> j;

    encodage_action_par_nom = j.value("actions", std::unordered_map<std::string, std::vector<float>>());
    label_effects = j.value("labels", std::unordered_map<std::string, std::vector<float>>());
}

