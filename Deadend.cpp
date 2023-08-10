#include <iostream>
#include <chrono>
#include <thread>
#include "Deadend.h"
#include "Utils.h"

using namespace std;

/*HINT: Uncomment for 4 dimension level
const bitset<16> Deadend::firstCol  ("1000100010001000");
const bitset<16> Deadend::lastCol   ("0001000100010001");
const bitset<16> Deadend::firstRow  ("1111000000000000");
const bitset<16> Deadend::lastRow   ("0000000000111111");*/

//HINT: Uncomment for 6 dimension level
const bitset<36> Deadend::firstCol  ("100000100000100000100000100000100000");
const bitset<36> Deadend::lastCol   ("000001000001000001000001000001000001");
const bitset<36> Deadend::firstRow  ("111111000000000000000000000000000000");
const bitset<36> Deadend::lastRow   ("000000000000000000000000000000111111");
bitset<36> Deadend::u_Corner        ("000100000100001100111100000000000000");
bitset<36> Deadend::u_FreeCorner    ("111000111000110000000000000000000000");
bitset<36> Deadend::r_Corner        ("100000100000110000111100000000000000");
bitset<36> Deadend::r_FreeCorner    ("011100011100001100000000000000000000");
bitset<36> Deadend::d_Corner        ("111100110000100000100000000000000000");
bitset<36> Deadend::d_FreeCorner    ("000000001100011100011100000000000000");
bitset<36> Deadend::l_Corner        ("111100001100000100000100000000000000");
bitset<36> Deadend::l_FreeCorner    ("000000110000111000111000000000000000");
bitset<36> Deadend::copyBitset;

//HINT: Uncomment for 8 dimension level
/*const bitset<64> Deadend::firstCol  ("1000000010000000100000001000000010000000100000001000000010000000");
const bitset<64> Deadend::lastCol   ("0000000100000001000000010000000100000001000000010000000100000001");
const bitset<64> Deadend::firstRow  ("1111111100000000000000000000000000000000000000000000000000000000");
const bitset<64> Deadend::lastRow   ("0000000000000000000000000000000000000000000000000000000011111111");
bitset<64> Deadend::u_Corner     ("0001000000010000001100001111000000000000000000000000000000000000");
bitset<64> Deadend::u_FreeCorner ("1110000011100000110000000000000000000000000000000000000000000000");
bitset<64> Deadend::r_Corner     ("1000000010000000110000001111000000000000000000000000000000000000");
bitset<64> Deadend::r_FreeCorner ("0111000001110000001100000000000000000000000000000000000000000000");
bitset<64> Deadend::d_Corner     ("1111000011000000100000001000000000000000000000000000000000000000");
bitset<64> Deadend::d_FreeCorner ("0000000000110000011100000111000000000000000000000000000000000000");
bitset<64> Deadend::l_Corner     ("1111000000110000000100000001000000000000000000000000000000000000");
bitset<64> Deadend::l_FreeCorner ("0000000011000000111000001110000000000000000000000000000000000000");
bitset<64> Deadend::copyBitset;*/


bool Deadend::HasDeadend(ZenBoard* zenBoard){
    
    if(CheckAlleys(zenBoard)){
        Statistics::alleys++;
        return true;
    }

    //Poor deadlock pattern
    if(CheckCorners(zenBoard)){
        Statistics::corners++;
        return true;
    }
        
    return false;
}

bool Deadend::CheckCorners(ZenBoard* zenBoard){
    
    //HINT: Uncomment for 4 dimension level
    //bitset<16> copyBitset(zenBoard->garden.to_ulong());*/
    //HINT: Uncomment for 6 dimension level
    bitset<36> copyBitset(zenBoard->garden.to_ulong());
    /*HINT: Uncomment for 8 dimension level*/
    //bitset<64> copyBitset(zenBoard->garden.to_ulong());

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
}


bool Deadend::CheckAlleys(ZenBoard* zenBoard){

    int sum = 0;
    int dimension = Utils::DIMENSION;
    int max = Utils::GetMax();

    //HINT: Uncomment for 4 dimension level
    //bitset<16> initBitset("0000000000000001"); 
    //HINT: Uncomment for 6 dimension level
    bitset<36> initBitset("000000000000000000000000000000000001"); 
    //HINT: Uncomment for 8 dimension level
    //bitset<64> initBitset("0000000000000000000000000000000000000000000000000000000000000001"); 

    for(int i = 0 ; i < max; i++){

        if(zenBoard->garden[i] != 0) {
            initBitset = initBitset<<1;
            sum = 0;
            continue;
        }

        //check up
        if((initBitset&firstRow).none())
            if(zenBoard->garden[i+dimension] == 1)
                sum++;

        //Down
        if((initBitset&lastRow).none())
            if(zenBoard->garden[i-dimension] == 1)
                sum++;

        //Left
        if((initBitset&firstCol).none())
            if(zenBoard->garden[i+1] == 1)
                sum++;

        //Right
        if((initBitset&lastCol).none())
            if(zenBoard->garden[i-1] == 1)
                sum++;

        if(sum >= 3)
            return true;

        initBitset = initBitset<<1;
        sum = 0;
    }
    
    return false;
}
