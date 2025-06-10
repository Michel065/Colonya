#ifndef _PERLIN_NOISE_H
#define _PERLIN_NOISE_H 

#include "../../Commun/includes.h"


class PerlinNoise {
private:
    std::vector<int> p;

    // Interpolation lissée : 6t^5 - 15t^4 + 10t^3
    float fade(float t) const {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    // Interpolation linéaire
    float lerp(float a, float b, float t) const {
        return a + t * (b - a);
    }

    // Produit scalaire entre le vecteur de gradient et (x, y)
    float grad(int hash, float x, float y) const {
        switch (hash & 0x3) {
            case 0: return  x + y;
            case 1: return -x + y;
            case 2: return  x - y;
            case 3: return -x - y;
            default: return 0; // jamais atteint
        }
    }

public:
    // Constructeur : génère la permutation pseudo-aléatoire
    PerlinNoise(unsigned int seed = 42) {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);

        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);

        // Double le tableau pour éviter l'overflow dans l'indexation
        p.insert(p.end(), p.begin(), p.end());
    }

    // Bruit de Perlin 2D (valeurs entre -1 et 1)
    float noise(float x, float y) const {
        // Trouver la cellule de grille
        int X = (int)std::floor(x) & 255;
        int Y = (int)std::floor(y) & 255;

        // Coordonnées locales dans la cellule (entre 0 et 1)
        float xf = x - std::floor(x);
        float yf = y - std::floor(y);

        // Appliquer la courbe de lissage
        float u = fade(xf);
        float v = fade(yf);

        // Récupérer les hash pour les coins
        int aa = p[p[X    ] + Y    ];
        int ab = p[p[X    ] + Y + 1];
        int ba = p[p[X + 1] + Y    ];
        int bb = p[p[X + 1] + Y + 1];

        // Calculer les produits scalaires pour chaque coin
        float x1 = lerp(grad(aa, xf, yf),     grad(ba, xf - 1, yf),     u);
        float x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);

        float result = lerp(x1, x2, v);
        return result; // ∈ [-1, 1]
    }

    // Version avec octaves : combine plusieurs niveaux de bruit
    float noise_octaved(float x, float y, int octaves = 4, float persistence = 0.5f) const {
        float total = 0.0f;
        float frequency = 1.0f;
        float amplitude = 1.0f;
        float max_value = 0.0f;

        for (int i = 0; i < octaves; ++i) {
            total += noise(x * frequency, y * frequency) * amplitude;
            max_value += amplitude;

            amplitude *= persistence;
            frequency *= 2.0f;
        }

        return total / max_value; // normalisé entre [-1, 1]
    }
};

#endif