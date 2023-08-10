#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <boost/functional/hash.hpp>
#include "ZenBoard.h"
#include "Vector2.h"
#include "Utils.h"
#include "Statistics.h"

boost::dynamic_bitset<> ZenBoard::GetWholeBoard(){
    return garden | player;
}

//Compute H, compute the minimum lines contained in the board
void ZenBoard::CompH(){

    Statistics::h_start = std::chrono::high_resolution_clock::now();

    h = 0;

    boost::dynamic_bitset<> gardenClone = garden;
    int index;
    //Count how many lines i can draw in the map
    for (int row = Utils::DIMENSION - 1; row >= 0; row--) {
        for (int col = Utils::DIMENSION - 1; col >= 0; col--) {

            index= row * Utils::DIMENSION + col;
            index = Utils::GetCorrectIndex(index);

            if(gardenClone[index] == 1) continue;
                
            else{
                //count in 35
                int freeRight = Utils::CountSpaces(gardenClone, index, Utils::right, 1);
                int freeBottom = Utils::CountSpaces(gardenClone, index, Utils::up, Utils::DIMENSION);

                //check which is major
                if(freeRight == 0 && freeBottom == 0)
                    continue;
                else{
                    if(freeRight > freeBottom)
                        Utils::GardenPaint(gardenClone, index, freeRight, 1);
                    else
                        Utils::GardenPaint(gardenClone, index, freeBottom, Utils::DIMENSION);

                    h++;
                }
            }
        }
    }

    //PrintBitset(garden);
    //PrintBitset(player);
    //getchar();
    Statistics::h_end = std::chrono::high_resolution_clock::now();
    Statistics::h_total += ((Statistics::h_end-Statistics::h_start).count()/1000000.0);

}

void ZenBoard::PrintBitset() {

    for (int row = Utils::DIMENSION - 1; row >= 0; row--) {
        for (int col = Utils::DIMENSION - 1; col >= 0; col--) {
            int index = row * Utils::DIMENSION + col;

            if (garden[index] == 1)
                cout << Utils::PLAYER;

            else if (garden[index] == 1)
                cout << Utils::FREE;

            else if (garden[index] == 1)
                cout << Utils::BUSY;

            else
                cout << Utils::NORMAL;

            cout << garden[index] << " ";
        }
        cout << endl;
    }

    cout << Utils::NORMAL;
    cout << "\n";
}


size_t ZenBoard::GetHashCode(){

    std::hash<boost::dynamic_bitset<>> hash_fn81;

    return ((hash_fn81(garden) ^ hash_fn81(player))%1000);
}


//Print board
void ZenBoard::Print() {

    boost::dynamic_bitset<> board(garden | player); 

    for (int row = Utils::DIMENSION - 1; row >= 0; row--) {
        for (int col = Utils::DIMENSION - 1; col >= 0; col--) {
            int index = row * Utils::DIMENSION + col;

            if (player[index] == 1)
                cout << Utils::PLAYER;

            else if (garden[index] == 1)
                cout << Utils::FREE;

            else if (garden[index] == 1)
                cout << Utils::BUSY;

            else
                cout << Utils::NORMAL;

            cout << board[index] << " ";
        }
        cout << endl;
    }

    cout << "\n";
}