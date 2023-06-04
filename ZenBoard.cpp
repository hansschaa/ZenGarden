#include <bitset>
#include "ZenBoard.h"
#include "Vector2.h"
#include "Utils.h"

bitset<36> ZenBoard::GetWholeBoard(){
    return garden | player;
}

