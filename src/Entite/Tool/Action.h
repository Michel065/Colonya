#ifndef ACTION_H
#define ACTION_H

#include "../../Commun/includes.h"

class Entite;
class Case;

struct Action {
    std::string nom;
    std::function<void(Entite&)> executer;
    Case* cible = nullptr;
    Action(const std::string& n, std::function<void(Entite&)> func, Case* c = nullptr)
        : nom(n), executer(func), cible(c) {}

    Action() = default;
};

#endif
