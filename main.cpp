#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <unistd.h>
#include <algorithm>
#include <stack>
#include <list>
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include "Statistics.h"
#include "ZenBoard.h"
#include "Utils.h"
#include "Vector2.h"
#include "MyAlgorithms.h"
#include "GameConfig.h"
#include "Deadlock.h"

using namespace std;



bool ProcessCommands(int argc, char* argv[], ZenBoard& zenBoard, GameConfig& gameConfig);
void RunGame(ZenBoard& zenBoard, GameConfig& gameConfig);
void ManualPlay(ZenBoard& zenBoard);
void IAPlay(ZenBoard& zenBoard, GameConfig gameConfig);
void DoManualMove(ZenBoard& ZenBoard, int currentIndex, Vector2<int> dir);
bool MoveIsOut(int index, Vector2<int> direction);


/*Ex:  
./zen boards/6x6/level1.txt 1 1 1 6 5000000
*/

int main(int argc, char* argv[]) {

    ZenBoard zenBoard;
    GameConfig gameConfig;

    if(!ProcessCommands(argc, argv, zenBoard, gameConfig)){
        cout << Utils::ERROR << "Problemas con el ingreso de parámetros!" << Utils::NORMAL << endl;
        return 0;
    }

    //RunGame(zenBoard, gameConfig);

    Deadlock::HasTunnel(gameConfig);


    /*Utils::GetNeighbours(zenBoard);
    int cont = 0;
    for (const auto& board : Utils::neighbours) {
        cont++;
        // Aquí accedemos a los miembros de cada elemento de ZenBoard y los imprimimos
        // Supongamos que hay un miembro llamado "value" en ZenBoard
        cout << cont << endl;
        Utils::PrintBoard(board);
        cout << "Rocas" << endl;
        Utils::PrintDynamicBitset(board.garden);
        cout << "Player" << endl;
        Utils::PrintDynamicBitset(board.player);
        cout << endl;
    }*/


    return 0;
}   

void RunGame(ZenBoard& zenBoard, GameConfig& gameConfig){

    if(gameConfig.gameMode==0){

        ZenBoard lastZenBoard;

        while(!Utils::IsWin(zenBoard)){
            
            if(!(lastZenBoard == zenBoard))
                Statistics::turn+=1;
                
            cout << Utils::INFO << "\n==== MENU DE JUEGO ==========================" << Utils::NORMAL << endl;
            cout <<Utils::ERROR << "Turn: " << Statistics::turn << Utils::NORMAL<< endl<<endl;
            lastZenBoard = zenBoard;
            ManualPlay(zenBoard);
            
        }
    }

    else
        IAPlay(zenBoard, gameConfig);

    if(zenBoard.garden.all()){
        Utils::PrintBoard(zenBoard);
        cout << Utils::BUSY << "Nivel completo!"<<Utils::NORMAL << endl;
    }
}

//Manual play process
void ManualPlay(ZenBoard& zenBoard){
    int id;
    Vector2<int> direction = Vector2(0,0);
    
    //If player is outside of the board
    if(Utils::IsPlayerOutside(zenBoard)){
        do{
            id = Utils::GetID(zenBoard);
        }while(id==-1);

        direction = Utils::GetDirectionFromEntry(id);
        Vector2<int> initialPoint = Utils::GetInitialIndex(id);
        int index = (Utils::GetMax()-1) - (initialPoint.i* Utils::DIMENSION + initialPoint.j);


        Utils::DoManualMove(zenBoard, index, direction);
    }

    else{

        int index = zenBoard.player.find_first();

        direction = Utils::GetDirection(zenBoard);
        if(MoveIsOut(index, direction))
            zenBoard.player.reset();
        else
            Utils::DoManualMove(zenBoard, index, direction);
    }
}

//IA play
void IAPlay(ZenBoard& zenBoard, GameConfig gameConfig){

    Utils::showPath = gameConfig.showPath;

    switch (gameConfig.algorithmType)
    {
        case 0:
            MyAlgorithms::BFS(zenBoard);
            Statistics::Print("BFS");
            break;
        case 1:
            MyAlgorithms::AStar(zenBoard);
            Statistics::Print("AStar");
            break;
        case 2:
            MyAlgorithms::IDAStar(zenBoard);
            Statistics::Print("IDAStar");
            break;
        default:
            break;
    }
}

//Method for check is index is outside
bool MoveIsOut(int index, Vector2<int> direction){

    if(direction.i !=0){
        if(index + (direction.i * Utils::DIMENSION) < 0 || 
        index + (direction.i * Utils::DIMENSION) >= Utils::GetMax()) 
            return true;
    }
    
    else if(direction.j !=0){
        index+=1;
        if((index % Utils::DIMENSION == 0 && direction.j == -1) || 
        ((index+5) % Utils::DIMENSION == 0 && direction.j == 1))
            return true;
    }

    return false;
}


//Process user arguments and setup player config 1 and 2
bool ProcessCommands(int argc, char* argv[], ZenBoard& zenBoard, GameConfig& gameConfig) {

    std::vector<std::string> args;

    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    std::cout << "Se ingresaron " << args.size() << " argumentos:" << std::endl;
    for (const auto& arg : args) {
        cout << arg << ", ";
    }
    cout << endl;

    string boardTxt = args[0], gardenstr, playerstr;
    std::ifstream archivo(boardTxt);
    std::list<char> char_list;
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            for (char c : linea) {
                char_list.push_back(c);
            }
        }
        archivo.close();
    } else {
        std::cerr << "Error: No se pudo abrir el archivo: " << boardTxt << std::endl;
        return false;
    }

    //Fill
    // Recorre la lista con un iterador
    for (auto it = char_list.begin(); it != char_list.end(); ++it){
        char c = *it;

        if(c== '1'){
             gardenstr+= '1';
             playerstr+='0';
        }

        else if(c=='2'){
            gardenstr+= '0';
            playerstr+= '1';
        }

        else{
            gardenstr+= '0';
            playerstr+= '0';
        }
    }

    //Set board
    boost::dynamic_bitset<> newGarden(gardenstr); 
    boost::dynamic_bitset<> newPlayer(playerstr); 

    zenBoard.garden = boost::dynamic_bitset<> (gardenstr);
    zenBoard.player = boost::dynamic_bitset<> (playerstr);

    //Others
    gameConfig.gameMode = stoi(args[1]);
    gameConfig.algorithmType = stoi(args[2]);
    gameConfig.showPath = stoi(args[3]);
    gameConfig.userLenght = stoi(args[4]);

    Utils::DIMENSION = gameConfig.userLenght;
    Statistics::maxMilliseconds = stoi(args[5]);

    cout << "Tablero ingresado: " << endl;
    Utils::PrintBoard(zenBoard);


    return true;
}