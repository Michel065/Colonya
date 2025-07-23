#ifndef MEMOIRE_IA_ENCODER_H
#define MEMOIRE_IA_ENCODER_H

#include "../../Commun/includes.h"
#include "EncodedAction.h"
#include "EncodedObservation.h"
#include "../Entite.h"

class MemoireIAEncoder {
private:
    int TAILLE_VECTOR_OBSERVATION = 6;
    int TAILLE_VECTOR_ACTION = 17;
    std::string filepath;
    std::string source_dir;

    
    std::unordered_map<std::string, std::vector<float>> encodage_action_par_nom;
    std::unordered_map<std::string, std::vector<float>> label_effects;

public:
    MemoireIAEncoder(const std::string& source_file,const std::string& name_creature="creature_1");
    ~MemoireIAEncoder();

    std::vector<float> encoder_observation(const Observation& obs, const Entite& creature);
    std::vector<float> encoder_action(const std::string& nom_action);

    EncodedAction encoder(const Action& action);
    EncodedObservation encoder(const Observation& obs, const Entite& creature);

    std::vector<float> vecteur_observation_par_defaut() const;
    std::vector<float> vecteur_action_par_defaut() const;

    void update_action_encoding(const std::string& nom, const std::vector<float>& vec);
    void update_label_effect(const std::string& label, const std::vector<float>& vec);
    EncodedObservation get_encoded_observation(const std::string& label) const;
    EncodedAction get_encoded_action(const std::string& nom) const ;
    
    const std::unordered_map<std::string, std::vector<float>>& get_encodage_action_map() const;
    const std::unordered_map<std::string, std::vector<float>>& get_label_effect_map() const;

    void save_to_json() const;
    void load_from_json();
};


#endif
