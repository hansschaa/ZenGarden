#include <iostream>
#include <bitset>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include "Utils.h"
#include "ZenBoard.h"

//Colors
const string Utils::ERROR = "\033[0;31m";
const string Utils::FREE = "\033[0;32m";
const string Utils::BUSY ("\033[0;33m");
const string Utils::INFO ("\033[0;35m");
const string Utils::PLAYER ("\033[0;31m");
const string Utils::NORMAL ("\033[0;37m");
const string Utils::OTHER ("\033[0;34m");


//Vectors
const Vector2<int> Utils::up = Vector2<int>(1,0);
const Vector2<int> Utils::right = Vector2<int>(0,1);
const Vector2<int> Utils::down = Vector2<int>(-1,0);
const Vector2<int> Utils::left = Vector2<int>(0,-1);


using namespace std;

unordered_set<ZenBoard, Utils::GetHashCode,  Utils::Equals> Utils::neighbours;
unordered_set<ZenBoard, Utils::GetHashCode,  Utils::Equals> Utils::visited;
unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals> Utils::map;
unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals> Utils::aStarCache;


unordered_map<ZenBoard, int, Utils::GetHashCode,  Utils::Equals> Utils::OPEN;
unordered_set<ZenBoard, Utils::GetHashCode,  Utils::Equals> Utils::CLOSE;

int Utils::showPath = 0; 

void Utils::GetNeighbours(ZenBoard zenBoard){

    //Reset neigbords
    neighbours.clear();

    //Player is on board
    if(zenBoard.player.any()){

        int currentIndex = zenBoard.player._Find_first();

        //Check up
        if(CanMove(zenBoard, currentIndex, up, Utils::DIMENSION, true)){
            PaintChild(zenBoard, currentIndex, up, Utils::DIMENSION);

        }

        //Check down
        if(CanMove(zenBoard, currentIndex, down, Utils::DIMENSION, true)){
            PaintChild(zenBoard, currentIndex, down, Utils::DIMENSION);
        }
        
        //check right
        if(CanMove(zenBoard, currentIndex, left, 1, true)){
            PaintChild(zenBoard, currentIndex, left, 1);
        }
        
        //Check left
        if(CanMove(zenBoard, currentIndex, right, 1, true)){
            PaintChild(zenBoard, currentIndex, right, 1);
        }
    }

    //Player is out of board
    else{
        //Up
        for(int i = 18 ; i >= 13; i--){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = 35 - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, up, Utils::DIMENSION, false)){
                PaintChild(zenBoard, currentIndex, up, Utils::DIMENSION);

            }
        }

        //Down
        for(int i = 1 ; i < 7; i++){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = 35 - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, down, Utils::DIMENSION, false)){
                PaintChild(zenBoard, currentIndex, down, Utils::DIMENSION);
            }
        }
        
        //Left
        for(int i = 7 ; i < 13; i++){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = 35 - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, right, 1, false)){
                PaintChild(zenBoard, currentIndex, right, 1);
            }
        }
        
        //Right
        for(int i = 24 ; i >= 19; i--){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = 35 - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, left, 1, false)){
                PaintChild(zenBoard, currentIndex, left, 1);
            }
        }
    }

    /*cout << Utils::ERROR << "---------------------------" << Utils::NORMAL << endl;
    cout << Utils::ERROR << "--> Boards" << Utils::NORMAL << endl;
    cout << Utils::ERROR << "--> Padre" << Utils::NORMAL << endl;
    Utils::PrintBoard(zenBoard);
    // Imprimir los valores del unordered_set
    for (const ZenBoard estado : Utils::neighbours) {
        // Acceder a los valores a través de los punteros
         cout << "########################" << endl;
        Utils::PrintBitset(estado.garden);
        Utils::PrintBitset(estado.player);
        Utils::PrintBoard(estado);
        cout << "########################" << endl;
    }
    cout << Utils::ERROR << "---------------------------" << Utils::NORMAL << endl;

    getchar();*/
}

void Utils::PaintChild(const ZenBoard& zenBoard, int currentIndex, Vector2<int> dir, int step) {
    ZenBoard child = zenBoard;
    IAPaint(child, currentIndex, dir, step);
    Utils::neighbours.insert(child);
}

void Utils::DoManualMove(ZenBoard& zenBoard, int currentIndex, Vector2<int> dir){
    
    //DOWN
    if(dir.i == -1)
        ManualPaint(zenBoard, currentIndex, dir, -Utils::DIMENSION);        
    
    //Up
    else if(dir.i == 1)
        ManualPaint(zenBoard, currentIndex, dir, Utils::DIMENSION);        

    //LEFT
    else if(dir.j == -1)
        ManualPaint(zenBoard, currentIndex, dir, 1); 

    //Right
    else if(dir.j == 1)
        ManualPaint(zenBoard, currentIndex, dir, -1);     
}

void Utils::ManualPaint(ZenBoard& zenBoard, int currentIndex, Vector2<int> direction, int step) {
    
    int i = 0;
    bool nextToObstacle = false;

    //GARDEN CHECK
    i = currentIndex;
    do {
        if (Utils::IsInside(i)) {
            if ((zenBoard.garden &~ zenBoard.player)[i] == 1) {
                nextToObstacle = true;
                break;
            }
        }

        zenBoard.garden.set(i, 1);

        i += step;
    } while (Utils::ManualGetEndPaintCondition(direction, i));

    if(direction.j == 1)
        zenBoard.garden.set(i,1);

    //PLAYER CHECK
    //El player estaba dentro del tablero
    if(nextToObstacle){
        zenBoard.player.reset();
        zenBoard.player.set(i-step, 1);
    }

    //El player no estaba, hay que verificar si el mov
    //Hizo que se quedara adentro
    else{
        zenBoard.player.reset();
    }
}

void Utils::IAPaint(ZenBoard& zenBoard, int currentIndex, Vector2<int> direction, int step) {

    int index = currentIndex;
    int dirFactor = direction.i != 0 ? direction.i : direction.j;
    bool nextToObstacle = false;

    //GARDEN CHECK
    do {
        if (Utils::IsInside(index)) {
            if ((zenBoard.garden &~ zenBoard.player)[index] == 1) {
                nextToObstacle = true;
                break;
            }
        }
        
        zenBoard.garden.set(index, 1);

        index += step*dirFactor;
    } while (Utils::GetEndPaintCondition(direction, index, step, dirFactor));

    //PLAYER CHECK
    //El player estaba dentro del tablero
    if(nextToObstacle){
        zenBoard.player.reset();
        zenBoard.player.set(index - (step*dirFactor), 1);
        zenBoard.lastDir = direction;
    }

    //El player no estaba, hay que verificar si el mov
    //Hizo que se quedara adentro
    else{
        zenBoard.player.reset();
        zenBoard.lastDir = Vector2<int>(0,0);
    }
}

//For ZenBoard heuristic
int Utils::CountSpaces(bitset<36>& gardenClone, int currentIndex, Vector2<int> direction, int step){
    
    int index = currentIndex;
    int dirFactor = 1;
    int count = 0;

    //Garden check
    do {
        if (Utils::IsInside(index)) {
            if (gardenClone[index] == 1) {
                break;
            }
        }
        
        index += step*dirFactor;

        count++;
    } while (Utils::GetEndPaintCondition(direction, index, step, dirFactor));

    return count;
}

void Utils::GardenPaint(bitset<36>& gardenClone, int currentIndex, int max, int step) {

    int index = currentIndex;
    int count = 0;

    //Garden paint
    do {
        gardenClone.set(index, 1);
        count++;
        index += step;
    } while (count < max);
}


bool Utils::CanMove(ZenBoard& zenBoard, int currentIndex, Vector2<int> direction, int step, bool isPlayerInsideBoard){
    

    if(direction.i != 0){
        if(direction.i*-1 == zenBoard.lastDir.i){
            return false;
        }
    }

    if(direction.j != 0){
        if(direction.j*-1 == zenBoard.lastDir.j){
            return false;
        }
    }

    if(!isPlayerInsideBoard){
        if(zenBoard.garden[currentIndex] == 1) {
            return false;
        }
    }

    else{

       if( direction.i != 0){
            if(zenBoard.garden[currentIndex + (direction.i*step)] == 1)
                return false;
        }
            
        if( direction.j != 0){
            if(zenBoard.garden[currentIndex + direction.j] == 1) 
                return false;
        }
    }

    return true;
}

bool Utils::GetEndPaintCondition(Vector2<int> direction, int index, int step, int dirFactor){
    if(direction.i == 1)  
        return index < Utils::GetMax();
    else if(direction.i == -1) 
        return index >= 0;
    else if(direction.j == -1)
        return (index - (step*dirFactor))%Utils::DIMENSION != 0;
    else if(direction.j == 1)
        return index%Utils::DIMENSION != 0;
    else
        return false;
}

bool Utils::ManualGetEndPaintCondition(Vector2<int> direction, int index){
    if(direction.i == 1)  
        return index < Utils::GetMax();
    else if(direction.i == -1) 
        return index >= 0;
    else 
        return index %Utils::DIMENSION != 0;
}

void Utils::PrintBoard(ZenBoard zenBoard) {

    std::bitset<36> board(zenBoard.garden | zenBoard.player); 

    for (int row = Utils::DIMENSION - 1; row >= 0; row--) {
        for (int col = Utils::DIMENSION - 1; col >= 0; col--) {
            int index = row * Utils::DIMENSION + col;

            if (zenBoard.player[index] == 1)
                cout << Utils::PLAYER;

            else if (zenBoard.garden[index] == 1)
                cout << Utils::FREE;

            else if (zenBoard.garden[index] == 1)
                cout << Utils::BUSY;

            else
                cout << Utils::NORMAL;

            cout << board[index] << " ";
        }
        cout << endl;
    }

    cout << "\n";
}

void Utils::PrintBitset(bitset<36> board) {

    for (int row = Utils::DIMENSION - 1; row >= 0; row--) {
        for (int col = Utils::DIMENSION - 1; col >= 0; col--) {
            int index = row * Utils::DIMENSION + col;

            if (board[index] == 1)
                cout << Utils::PLAYER;

            else if (board[index] == 1)
                cout << Utils::FREE;

            else if (board[index] == 1)
                cout << Utils::BUSY;

            else
                cout << Utils::NORMAL;

            cout << board[index] << " ";
        }
        cout << endl;
    }

    cout << Utils::NORMAL;
    cout << "\n";
}

void Utils::PrintDynamicBitset(boost::dynamic_bitset<> board) {

    for (int row = Utils::DIMENSION - 1; row >= 0; row--) {
        for (int col = Utils::DIMENSION - 1; col >= 0; col--) {
            int index = row * Utils::DIMENSION + col;

            if (board[index] == 1)
                cout << Utils::PLAYER;

            else if (board[index] == 1)
                cout << Utils::FREE;

            else if (board[index] == 1)
                cout << Utils::BUSY;

            else
                cout << Utils::NORMAL;

            cout << board[index] << " ";
        }
        cout << endl;
    }

    cout << Utils::NORMAL;
    cout << "\n";
}

void Utils::PrintBoardWIndexs(ZenBoard zenBoard) {

    std::bitset<36> board(zenBoard.garden | zenBoard.player); 

    int cont = 1;
    int cont2 = 7;

    for(int i = 1; i <= Utils::DIMENSION ; i++){
        if(i == 1)
            cout << Utils::OTHER << "   "<< cont << Utils::NORMAL;
        else
            cout << Utils::OTHER << "   "<< cont << Utils::NORMAL;

        cont++;
    }
        

    cout << "\n";
    
    cont = 19;

    for (int row = Utils::DIMENSION - 1; row >= 0; row--) {
       
        cout << Utils::OTHER << (cont + row) << " " << Utils::NORMAL;

        for (int col = Utils::DIMENSION - 1; col >= 0; col--) {
            int index = row * Utils::DIMENSION + col;

            if (zenBoard.player[index] == 1)
                cout << Utils::PLAYER;

            else if (zenBoard.garden[index] == 1)
                cout << Utils::FREE;

            else if (zenBoard.garden[index] == 1)
                cout << Utils::BUSY;

            else
                cout << Utils::NORMAL;

            if(col != 0)
                cout << board[index] << "   ";
            else
                cout << board[index] << " ";
        }

        cout << Utils::OTHER << cont2 << Utils::NORMAL;
        cout << endl;
        cont2++;
    }

    cont2 = 18;

    for(int i = 1; i <= Utils::DIMENSION ; i++){
        if(i == 1)
            cout << Utils::OTHER << "  "<< cont2 << Utils::NORMAL;
        else
            cout << Utils::OTHER << "  "<< cont2 << Utils::NORMAL;
        
        cont2--;
    }

    cout << "\n";
}

Vector2<int> Utils::GetInitialIndex(int id){
    if(id >= 1 && id <=6) return Vector2(0,id-1);
    else if(id >= 7 && id <=12) return Vector2(abs(7-id), 5);
    else if(id >= 13 && id <=18) return Vector2(5, abs(18-id));
    else if(id >= 19 && id <=24) return Vector2(24-id, 0);
    else return Vector2(0, 0);

}

int Utils::GetID(ZenBoard zenBoard){
    string opcion;
    int id = 0;
    Utils::PrintBoardWIndexs(zenBoard);
    do{
        cout << Utils::INFO << "-> Ingresa el id por donde deseas entrar a rastrillar: "   << Utils::NORMAL;
        cin >> opcion;
        id = std::stoi(opcion);  
    }
    while(id <1 || id > 24);

    cout << endl;

    return id;
}

Vector2<int> Utils::GetDirection(ZenBoard zenBoard){
    string opcion;
    
    int num1_int, num2_int;
    std::string num1,num2;
    Utils::PrintBoardWIndexs(zenBoard);
    do{
        cout << Utils::INFO << "-> Ingresa la dirección en formato x,y (ej: 0,1 o -1,0): "   << Utils::NORMAL;
        cin >> opcion;

        if(opcion.size() > 3){
            size_t pos = opcion.find(',');
            std::string valor1 = opcion.substr(0, pos);
            std::string valor2 = opcion.substr(pos + 1);

            // Convertir las cadenas a int
            num1_int = std::stoi(valor1);
            num2_int = std::stoi(valor2);
        }

        else{
            num1 = opcion.substr(0,1);  
            num2 = opcion.substr(2,3);  
            num1_int = std::stoi(num1);  
            num2_int = std::stoi(num2);  
        }
    }
    while(!((num1_int == 0 && num2_int == 1) || (num1_int == 0 && num2_int == -1) || 
    (num1_int == 1 && num2_int == 0) || (num1_int == -1 && num2_int == 0)));

    return Vector2<int>(num1_int, num2_int);
}

Vector2<int> Utils::GetDirectionFromEntry(int id){
   if(id >= 1 && id <= 6) return Vector2(-1,0);
   if(id >= 7 && id <= 12) return Vector2(0,-1);
   if(id >= 13 && id <= 18) return Vector2(1,0);
   if(id >= 19 && id <= 24) return Vector2(0,1);
   else return Vector2(0,0);
}

int Utils::GetCorrectIndex(int index){

    int max = GetMax();
    index = max-index-1;

    return index;
}

int Utils::GetCorrectIndex(Vector2<int> entry){

    int max = GetMax();
    int index = entry.i*Utils::DIMENSION + entry.j;
    index = max-index-1;

    return index;
}

bool Utils::IsPlayerOutside(ZenBoard zenBoard){
    return zenBoard.player.none();
}

int Utils::GetMax(){
    return Utils::DIMENSION * Utils::DIMENSION;
}

bool Utils::IsWin(ZenBoard& zenBoard){
    return zenBoard.garden.all();
}

bool Utils::IsInside(int index){
    return index >= 0 && index < Utils::GetMax();
}
