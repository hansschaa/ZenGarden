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

/*bitset<36> Deadend::u_Corner     ("000100000100001100111100000000000000");
bitset<36> Deadend::u_FreeCorner ("111000111000110000000000000000000000");

bitset<36> Deadend::r_Corner     ("100000100000110000111100000000000000");
bitset<36> Deadend::r_FreeCorner ("011100011100001100000000000000000000");

bitset<36> Deadend::d_Corner     ("111100110000100000100000000000000000");
bitset<36> Deadend::d_FreeCorner ("000000001100011100011100000000000000");

bitset<36> Deadend::l_Corner     ("111100001100000100000100000000000000");
bitset<36> Deadend::l_FreeCorner ("000000110000111000111000000000000000");

bitset<36> Deadend::copyBitset;*/


bool Deadend::HasDeadend(ZenBoard& zenBoard){
    
    if(CheckTunnels(zenBoard)){
        Statistics::tunnels++;
        return true;
    }

    //Poor deadlock pattern
    /*if(CheckCorners(zenBoard)){
        Statistics::corners++;
        return true;
    }*/
        
    return false;
}

/*bool Deadend::CheckCorners(ZenBoard& zenBoard){
    
    bitset<36> copyBitset(zenBoard.garden.to_ulong());

    int max = Utils::DIMENSION-3;
    int val;

    for(int i = 0 ; i < max ; i++){
        for(int j = 0 ; j < max; j++){

            val = j+3*i;

            if(((r_Corner>>val) &~ copyBitset).none())
                if((r_FreeCorner>>val &~ copyBitset) == (r_FreeCorner>>val))
                    return true;
            

            if(((u_Corner>>val) &~ copyBitset).none())
                if((u_FreeCorner>>val &~ copyBitset) == (u_FreeCorner>>val))
                    return true;
          

            if(((d_Corner>>val) &~ copyBitset).none())
                if((d_FreeCorner>>val &~ copyBitset) == (d_FreeCorner>>val))
                    return true;
               

            if(((l_Corner>>val) &~ copyBitset).none())
                if((l_FreeCorner>>val &~ copyBitset) == (l_FreeCorner>>val))
                    return true;
        }
    }

    return false;
}*/


bool Deadend::CheckTunnels(ZenBoard& zenBoard){

    int sum = 0;
    int dimension = Utils::DIMENSION;
    int max = Utils::GetMax();

    //HACK
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
