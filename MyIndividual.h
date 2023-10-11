#ifndef ZEN_MYINDIVIDUAL_H
#define ZEN_MYINDIVIDUAL_H

#include "ZenBoard.h"
#include "Utils.h"


using namespace std;

class MyIndividual{
    public:
        int moveMutation = 50;
        float addMutation = 95;
        int removeMutation = 100;
        int maxRocks = 6;

        ZenBoard zenBoard;
        int fitness;

        bool Mutate();
        bool MoveMutation();
        bool AddMutation();
        bool RemoveMutation();
};

#endif //ZEN_MYINDIVIDUAL_H