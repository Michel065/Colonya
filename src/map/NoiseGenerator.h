#ifndef _NOISE_GENERATOR_H
#define _NOISE_GENERATOR_H 

#include "../Commun/includes.h"
#include "./NoiseMethode/PerlinNoise.h"

struct NoiseParam
{
    unsigned int seed=42;
    int octaves=4; 
    float persistence=0.5f;
    float scale_altitude = 0.05f;
    float scale_humidity = 0.05f;
    int decalage_humidity = 1000;
    
    NoiseParam(unsigned int s=42):seed(s){}
};

inline void to_json(json& j, const NoiseParam& n) {
    auto conv_f_vers_i = [](float val) -> int { // 5 decimale
        return std::round(val * 100000);
    };
    j = json{
        {"seed", n.seed},
        {"octaves", n.octaves},
        {"persistence", conv_f_vers_i(n.persistence)},
        {"scale_altitude", conv_f_vers_i(n.scale_altitude)},
        {"scale_humidity", conv_f_vers_i(n.scale_humidity)},
        {"decalage_humidity", n.decalage_humidity}
    };
}

inline void from_json(const json& j, NoiseParam& n) {
    auto conv_i_vers_f = [](int val) -> float { // 5 decimale
        return std::round(val / 100000);
    };
    j.at("seed").get_to(n.seed);
    j.at("octaves").get_to(n.octaves);
    j.at("decalage_humidity").get_to(n.decalage_humidity);
    int p, sa, sh;
    j.at("persistence").get_to(p);
    j.at("scale_altitude").get_to(sa);
    j.at("scale_humidity").get_to(sh);

    n.persistence = conv_i_vers_f(p);
    n.scale_altitude = conv_i_vers_f(sa);
    n.scale_humidity = conv_i_vers_f(sh);
}

class NoiseGenerator {
private:
    PerlinNoise noise;
    NoiseParam* param;

public:
    NoiseGenerator(NoiseParam* par ):param(par){}
    ~NoiseGenerator(){
        if(param)delete param;
    }

    float altitude(int x, int y) const {
        return noise.noise_octaved(x * param->scale_altitude, y * param->scale_altitude, param->octaves, param->persistence);
    }

    float humidity(int x, int y) const {
        return noise.noise_octaved(x * param->scale_humidity + param->decalage_humidity, y * param->scale_humidity + param->decalage_humidity, param->octaves, param->persistence);
    }

    NoiseParam* get_param(){return param;}

};

#endif
