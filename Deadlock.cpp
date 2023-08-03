#include <iostream>
#include <chrono>
#include <thread>
#include "Deadlock.h"
#include "Utils.h"


using namespace std;


boost::dynamic_bitset<> u_TunnelHead = 
boost::dynamic_bitset<> (string("010000101000000000000000000000000000"));
boost::dynamic_bitset<> r_TunnelHead = 
boost::dynamic_bitset<> (string("100000010000100000000000000000000000"));
boost::dynamic_bitset<> d_TunnelHead = 
boost::dynamic_bitset<> (string("101000010000000000000000000000000000"));
boost::dynamic_bitset<> l_TunnelHead = 
boost::dynamic_bitset<> (string("010000100000010000000000000000000000"));

bool Deadlock::HasTunnel(GameConfig& gameConfig, ZenBoard& zenBoard){

    bool hasTunnel = false;

    hasTunnel = CheckTunnel(gameConfig, u_TunnelHead, 1,2,2,1, Utils::GetMax()-8, zenBoard);
    if(hasTunnel) return true;

    hasTunnel = CheckTunnel(gameConfig, r_TunnelHead, 2,1,1,1,  Utils::GetMax()-7, zenBoard);
    if(hasTunnel) return true;
    
    hasTunnel = CheckTunnel(gameConfig, d_TunnelHead, 1,2,2,1, Utils::GetMax()-2, zenBoard);
    if(hasTunnel) return true;

    hasTunnel = CheckTunnel(gameConfig, l_TunnelHead, 2,1,1,1, Utils::GetMax()-8, zenBoard);
    if(hasTunnel) return true;

    /*int x =0;

    thread t1(CheckTunnel, gameConfig, u_TunnelHead, 1, 2, 2, 1, Utils::GetMax() - 8, zenBoard,std::ref(x));
    thread t2(CheckTunnel, gameConfig, r_TunnelHead, 2, 1, 1, 1, Utils::GetMax() - 7, zenBoard,std::ref(x));
    thread t3(CheckTunnel, gameConfig, d_TunnelHead, 1, 2, 2, 1, Utils::GetMax() - 2, zenBoard,std::ref(x));
    thread t4(CheckTunnel, gameConfig, l_TunnelHead, 2, 1, 1, 1, Utils::GetMax() - 8, zenBoard,std::ref(x));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    if(x > 0){

          return true;
    }*/
      
    Reset();

    return false;
}


bool Deadlock::CheckTunnel(GameConfig& gameConfig, boost::dynamic_bitset<> bitset, 
        int iGap, int jGap, int iJump, int jJump, int emptyIndex, 
        ZenBoard& zenBoard){
    for(int i = 0 ; i < gameConfig.userLenght-iGap; i++){
        for(int j = 0 ; j < gameConfig.userLenght-jGap; j++){

            //bitset.set(emptyIndex,1);

            //Check
            if(zenBoard.GetWholeBoard()[emptyIndex]==0 && (bitset&~zenBoard.garden).none()){
                return true;
            }
                

            //Utils::PrintDynamicBitset(bitset);
            //bitset.set(emptyIndex,0);

            //next
            bitset >>= jJump;
            
            emptyIndex-=jJump;

            //getchar();
            
        }
        bitset >>= iJump;
        emptyIndex-=iJump;
    }
    return false;
}

void Deadlock::Reset(){
    u_TunnelHead = boost::dynamic_bitset<> (string("010000101000000000000000000000000000"));
    r_TunnelHead = boost::dynamic_bitset<> (string("100000010000100000000000000000000000"));
    d_TunnelHead = boost::dynamic_bitset<> (string("101000010000000000000000000000000000"));
    l_TunnelHead = boost::dynamic_bitset<> (string("010000100000010000000000000000000000"));
}