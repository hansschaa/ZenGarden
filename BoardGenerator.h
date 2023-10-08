#ifndef ZEN_BOARDGENERATOR_H
#define ZEN_BOARDGENERATOR_H

#include <chrono>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include <bitset>
#include <stdlib.h>     /* srand, rand */
#include "GameConfig.h"
#include "TunnelInfo.h"
#include "ZenBoard.h"
#include "MyAlgorithms.h"


using namespace std;

class BoardGenerator{
    public:
        static int mu;
        static int lambda;
        static int iterations;

        //Templates
        // std::bitset<36> pointTemplate;

        void Run();
        vector<std::bitset<36>> GetInitialPopulation();
};

#endif //ZEN_BOARDGENERATOR_H