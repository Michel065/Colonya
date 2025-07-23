#ifndef ENCODED_ACTION_H
#define ENCODED_ACTION_H

#include "Action.h"

struct EncodedAction {
    std::string nom;
    std::vector<float> features;
};

#endif
