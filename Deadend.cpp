#include <iostream>
#include <chrono>
#include <thread>
#include "Deadend.h"
#include "Utils.h"

using namespace std;

const bitset<36> Deadend::firstCol  ("100000100000100000100000100000100000");
const bitset<36> Deadend::lastCol   ("000001000001000001000001000001000001");
const bitset<36> Deadend::firstRow  ("111111000000000000000000000000000000");
const bitset<36> Deadend::lastRow   ("000000000000000000000000000000111111");

bool Deadend::HasDeadend(ZenBoard& zenBoard){
    return CheckTunnels(zenBoard);
}

bool Deadend::CheckTunnels(ZenBoard& zenBoard){

    int sum = 0;
    int dimension = Utils::DIMENSION;
    int max = Utils::GetMax();
    bitset<36> initBitset("000000000000000000000000000000000001"); 

    for(int i = 0 ; i < max; i++){

        if(zenBoard.garden[i] != 0) {
            initBitset = initBitset<<1;
            sum = 0;
            continue;
        }

        //check up
        if((initBitset&firstRow).none())
            if(zenBoard.garden[i+dimension] == 1){
                //cout << "up" << endl;
                sum++;
            }

        //Down
        if((initBitset&lastRow).none())
            if(zenBoard.garden[i-dimension] == 1){
                //cout << "down" << endl;
                sum++;
            }

        //Left
        if((initBitset&firstCol).none())
            if(zenBoard.garden[i+1] == 1){
                //cout << "left" << endl;
                sum++;
            }

        //Right
        if((initBitset&lastCol).none())
            if(zenBoard.garden[i-1] == 1){
                //cout << "right" << endl;
                sum++;
            }

        if(sum >= 3)
            return true;

        initBitset = initBitset<<1;
        sum = 0;
    }
    
    return false;
}
