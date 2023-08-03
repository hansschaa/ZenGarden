#include <iostream>
#include <chrono>
#include "Deadlock.h"
#include "Utils.h"


using namespace std;

boost::dynamic_bitset<> u_TunnelHead = 
boost::dynamic_bitset<> (string("111000101000000000000000000000000000"));
boost::dynamic_bitset<> r_TunnelHead = 
boost::dynamic_bitset<> (string("110000010000110000000000000000000000"));
boost::dynamic_bitset<> d_TunnelHead = 
boost::dynamic_bitset<> (string("101000111000000000000000000000000000"));
boost::dynamic_bitset<> l_TunnelHead = 
boost::dynamic_bitset<> (string("110000100000110000000000000000000000"));

bool Deadlock::HasTunnel(GameConfig gameConfig){

    int dimension = 3;
    for(int i = 0 ; i < gameConfig.userLenght-dimension; i++){

    }

    return false;
}

void Deadlock::Reset(){
    u_TunnelHead = boost::dynamic_bitset<> (string("111000101000000000000000000000000000"));
    r_TunnelHead = boost::dynamic_bitset<> (string("110000010000110000000000000000000000"));
    d_TunnelHead = boost::dynamic_bitset<> (string("101000111000000000000000000000000000"));
    l_TunnelHead = boost::dynamic_bitset<> (string("110000100000110000000000000000000000"));
}