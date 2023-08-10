#include <iostream>
#include <bitset>
#include <vector>

#include <boost/dynamic_bitset.hpp>
#include "Utils.h"
#include "ZenBoard.h"
#include "Deadend.h"

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

unordered_set<ZenBoard*, Utils::GetHashCode,  Utils::Equals> Utils::neighbours;
unordered_set<ZenBoard*, Utils::GetHashCode,  Utils::Equals> Utils::deadlocksTable;

//TT
TTEntry Utils::TT[1000000];

//unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals> Utils::map;
//unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals> Utils::aStarCache;
//unordered_map<ZenBoard, int, Utils::GetHashCode,  Utils::Equals> Utils::OPEN;
//unordered_set<ZenBoard, Utils::GetHashCode,  Utils::Equals> Utils::CLOSE;

int Utils::showPath = 0; 
int Utils::DIMENSION = 0;
GameConfig Utils::gameConfig;

//Get Neighbours from a zenBoard
void Utils::GetNeighbours(ZenBoard zenBoard, int newG){

    Statistics::gen_start = std::chrono::high_resolution_clock::now();

    //Reset neigbords
    neighbours.clear();

    //Player is on board
    if(zenBoard.player.any()){

        int currentIndex = zenBoard.player._Find_first();
        
        //Check up
        if(CanMove(zenBoard, currentIndex, up, Utils::DIMENSION, true)){
            PaintChild(zenBoard, currentIndex, up, Utils::DIMENSION, newG);
        }

        //Check down
        if(CanMove(zenBoard, currentIndex, down, Utils::DIMENSION, true)){
            PaintChild(zenBoard, currentIndex, down, Utils::DIMENSION, newG);
        }
        
        //check right
        if(CanMove(zenBoard, currentIndex, left, 1, true)){
            PaintChild(zenBoard, currentIndex, left, 1, newG);
        }
        
        //Check left
        if(CanMove(zenBoard, currentIndex, right, 1, true)){
            PaintChild(zenBoard, currentIndex, right, 1, newG);
        }
    }

    //Player is out of board
    else{
        //Up
        for(int i = (Utils::DIMENSION*3) ; i >= ((Utils::DIMENSION*2)+1); i--){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = (Utils::GetMax()-1) - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, up, Utils::DIMENSION, false)){
                PaintChild(zenBoard, currentIndex, up, Utils::DIMENSION, newG);
            }
        }

        //Down
        for(int i = 1 ; i < Utils::DIMENSION+1; i++){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = (Utils::GetMax()-1)  - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, down, Utils::DIMENSION, false)){
                PaintChild(zenBoard, currentIndex, down, Utils::DIMENSION, newG);
            }
        }
        
        //Left
        for(int i = Utils::DIMENSION+1 ; i < ((Utils::DIMENSION*2)+1); i++){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = (Utils::GetMax()-1)  - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, right, 1, false)){
                PaintChild(zenBoard, currentIndex, right, 1, newG);
            }
        }
        
        //Right
        for(int i = (Utils::DIMENSION*4) ; i >= ((Utils::DIMENSION*3)+1); i--){
            Vector2<int> initialPoint = GetInitialIndex(i);
            int currentIndex = (Utils::GetMax()-1) - (initialPoint.i* Utils::DIMENSION + initialPoint.j); 
            if(CanMove(zenBoard, currentIndex, left, 1, false)){
                PaintChild(zenBoard, currentIndex, left, 1, newG);
            }
        }
    }

    Statistics::gen_end = std::chrono::high_resolution_clock::now();
    Statistics::gen_total += ((Statistics::gen_end-Statistics::gen_start).count()/1000000.0);
}

//Create a new succesor from zenBoard whit the new move
void Utils::PaintChild(const ZenBoard& zenBoard, int currentIndex, Vector2<int> dir, int step, int newG) {
    
    ZenBoard* child = new ZenBoard(zenBoard);

    IAPaint(*child, currentIndex, dir, step);
    child->g = newG;
 
    //Insert in neig
    Utils::neighbours.insert(child);
}

void Utils::PrintTT(){
    // Imprimir los datos del array
    for (int i = 0; i < 1000; ++i) {
        auto board = Utils::TT[i];
        cout << "Tablero: " << i << endl;
        cout << "Bound: " <<  board._bound << endl;
        Utils::PrintBoard(board._zenBoard);
    }
}

//Move for not move player
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

//Paint for not IA player
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
    //cout << "-> IAPaint" << endl;
    int index = currentIndex;
    int dirFactor = direction.i != 0 ? direction.i : direction.j;
    bool nextToObstacle = false;

    //GARDEN CHECK
    do {
        if (Utils::IsInside(index)) {
            if ((zenBoard.garden)[index] == 1) {
                zenBoard.garden.set(index - step*dirFactor, 0);
                //Utils::PrintDynamicBitset(zenBoard.garden);
                //getchar();

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

//Garden paint for heuristic
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

//Check if new move is legal
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

        if(currentIndex < 0 || currentIndex >= Utils::GetMax())
            return false;

        if(zenBoard.garden[currentIndex] == 1) {
            return false;
        }
    }

    else{

        if( direction.i != 0){
            if(currentIndex + (direction.i*step) < 0 || currentIndex + (direction.i*step) >= Utils::GetMax()){
                return false;
            }

            if(zenBoard.garden[currentIndex + (direction.i*step)] == 1){
                return false;
            }
                
        }
            
        if( direction.j != 0 ){
            if(currentIndex + direction.j < 0 || currentIndex + direction.j >= Utils::GetMax()){
                return false;
            }

            if(zenBoard.garden[currentIndex + direction.j] == 1) {
                return false;
            }
                
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
    else{
        return false;
    }
       
}


bool Utils::ManualGetEndPaintCondition(Vector2<int> direction, int index){
    if(direction.i == 1)  
        return index < Utils::GetMax();
    else if(direction.i == -1) 
        return index >= 0;
    else 
        return index %Utils::DIMENSION != 0;
}

void Utils::PrintBoard(ZenBoard zenBoard)
{

    bitset<36> board(zenBoard.garden | zenBoard.player); 

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

    cout << Utils::NORMAL;

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

    bitset<36> board(zenBoard.garden | zenBoard.player); 

    int cont = 1;
    int cont2 = Utils::DIMENSION+1;

    for(int i = 1; i <= Utils::DIMENSION ; i++){
        if(i == 1)
            cout << Utils::OTHER << "   "<< cont << Utils::NORMAL;
        else
            cout << Utils::OTHER << "   "<< cont << Utils::NORMAL;

        cont++;
    }
        

    cout << "\n";
    
    cont = (Utils::DIMENSION*3)+1;

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

    cont2 = Utils::DIMENSION*3;

    for(int i = 1; i <= Utils::DIMENSION ; i++){
        if(i == 1)
            cout << Utils::OTHER << "  "<< cont2 << Utils::NORMAL;
        else
            cout << Utils::OTHER << "  "<< cont2 << Utils::NORMAL;
        
        cont2--;
    }

    cout << "\n";
}

//Get direction based in the column or row index
Vector2<int> Utils::GetInitialIndex(int id){
    if(id >= 1 && id <=Utils::DIMENSION) return Vector2(0,id-1);
    else if(id >= (Utils::DIMENSION+1) && id <= (Utils::DIMENSION*2)) return Vector2(abs((Utils::DIMENSION+1)-id), (Utils::DIMENSION-1));
    else if(id >= (Utils::DIMENSION*2)+1 && id <=(Utils::DIMENSION*3)) return Vector2((Utils::DIMENSION-1), abs(Utils::DIMENSION*3)-id);
    else if(id >= (Utils::DIMENSION*3)+1 && id <=(Utils::DIMENSION*4)) return Vector2((Utils::DIMENSION*4)-id, 0);
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
    while(id <1 || id > Utils::DIMENSION*4);

    cout << endl;

    return id;
}

//Get direction input
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

    if(id >= 1 && id <=Utils::DIMENSION) return Vector2(-1,0);
    else if(id >= (Utils::DIMENSION+1) && id <= (Utils::DIMENSION*2)) return Vector2(0,-1);
    else if(id >= (Utils::DIMENSION*2)+1 && id <=(Utils::DIMENSION*3)) return Vector2(1,0);
    else if(id >= (Utils::DIMENSION*3)+1 && id <=(Utils::DIMENSION*4)) return Vector2(0,1);
     else return Vector2(0,0);
}

//Get correct index, invert the index for proccess board
int Utils::GetCorrectIndex(int index){

    int max = GetMax();
    index = max-index-1;

    return index;
}

//Get correct index, invert the index for proccess board
int Utils::GetCorrectIndex(Vector2<int> entry){

    int max = GetMax();
    int index = entry.i*Utils::DIMENSION + entry.j;
    index = max-index-1;

    return index;
}

//Check if player is outside
bool Utils::IsPlayerOutside(ZenBoard zenBoard){
    return zenBoard.player.none();
}

//Get max index
int Utils::GetMax(){
    return Utils::DIMENSION * Utils::DIMENSION;
}

//Check if board is a win state
bool Utils::IsWin(ZenBoard& zenBoard){
    return zenBoard.garden.all();
}

//Check if index is legal
bool Utils::IsInside(int index){
    return index >= 0 && index < Utils::GetMax();
}

//TT Methods
TTEntry Utils::TTLookup(ZenBoard* zenBoard)
{
    return Utils::TT[zenBoard->GetHashCode() % 1000000];
}

void Utils::TTSave(ZenBoard zenBoard, int bound)
{
    TT[zenBoard.GetHashCode()%1000000] = TTEntry(zenBoard, bound);
}