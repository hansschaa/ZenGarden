#include <bitset>
#include "ZenBoard.h"
#include "Vector2.h"
#include "Utils.h"

bitset<36> ZenBoard::GetWholeBoard(){
    return garden | player;
}

void ZenBoard::CompH(){
    h = 0;

    bitset<36> gardenClone = garden;
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
}

void ZenBoard::Print() {

    std::bitset<36> board(garden | player); 

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