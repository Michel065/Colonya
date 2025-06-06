#ifndef _NOISE_GENERATOR_H
#define _NOISE_GENERATOR_H 

#include "../includes.h"
#include "PerlinNoise.h"

struct NoiseParam
{
    unsigned int seed=42;
    int octaves=4; 
    float persistence=0.5f;
    float scale_altitude = 0.05;
    float scale_humidity = 0.05;
    int decalage_humidity = 1000;
    
    NoiseParam(unsigned int s):seed(s){}
};

inline void to_json(json& j, const NoiseParam& n) {
    j = json{
        {"seed", n.seed},
        {"octaves", n.octaves},
        {"persistence", n.persistence},
        {"scale_altitude", n.scale_altitude},
        {"scale_humidity", n.scale_humidity},
        {"decalage_humidity", n.decalage_humidity}
    };
}

inline void from_json(const json& j, NoiseParam& n) {
    j.at("seed").get_to(n.seed);
    j.at("octaves").get_to(n.octaves);
    j.at("persistence").get_to(n.persistence);
    j.at("scale_altitude").get_to(n.scale_altitude);
    j.at("scale_humidity").get_to(n.scale_humidity);
    j.at("decalage_humidity").get_to(n.decalage_humidity);
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
