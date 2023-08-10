#include <unordered_set>
#include <iostream>
#include <chrono>
#include <stack>
#include <limits.h>
#include "Statistics.h"
#include "MyAlgorithms.h"
#include "Utils.h"

using namespace std;

//BFS Algorithm
void MyAlgorithms::BFS(ZenBoard& zenBoard){
    
    /*cout << "-> Empezando BFS..." << endl;
    Statistics::start = std::chrono::high_resolution_clock::now();
    queue<ZenBoard> queue;     
    queue.push(zenBoard);    

    //Update visited nodes whit root             
    Utils::visited.insert(zenBoard);

    //Parents cache 
    Utils::map.insert({zenBoard, zenBoard});    

    //Check nodes
    while (!queue.empty()) {

        ZenBoard currentBoard = queue.front();
        queue.pop();
       
        //Check if currentBoard is a win state
        if (Utils::IsWin(currentBoard)) {
            Statistics::end = std::chrono::high_resolution_clock::now();
            cout << Utils::FREE << "Solution found..." << Utils::NORMAL << endl;

            if(Utils::showPath)
                ShowMoves(currentBoard, Utils::map);
    
            return;
        }

        // Get neighbours 
        Utils::GetNeighbours(currentBoard, 1);  

        for (ZenBoard neighbour : Utils::neighbours) {
            Utils::map.insert({neighbour, currentBoard}); 
            if (Utils::visited.find(neighbour) == Utils::visited.end()) {
                queue.push(neighbour);                               
                Utils::visited.insert(neighbour);      
            }
        }

        Statistics::totalNodesExpanded++;
    }

    cout << Utils::ERROR << "Not solution..." << Utils::NORMAL << endl;*/
}

//AStar algorithm
void MyAlgorithms::AStar(ZenBoard& zenBoard){
    /*
    cout << "-> Empezando AStar.." << endl;
    Statistics::start = std::chrono::high_resolution_clock::now();

    //Sort ascendent
    struct ZenBoardComparator {
        bool operator()(const ZenBoard& zenBoard1, const ZenBoard& zenBoard2) const {

            // Orden ascendente basado en la variable f
            return zenBoard1.g + zenBoard1.h > zenBoard2.g + zenBoard2.h;
        }
    };

    priority_queue<ZenBoard, vector<ZenBoard>, ZenBoardComparator> openQueue;
    
    zenBoard.g = 0;
    zenBoard.CompH();

    //For parents
    Utils::aStarCache.insert({zenBoard, zenBoard}); 

    //Insert root node in open priority queue
    openQueue.push(zenBoard);

    while(!openQueue.empty()){

        //Get the min value in priority queue
        ZenBoard currentBoard = openQueue.top();
		openQueue.pop();

        // Si el nodo estaba en la lista cerrada con un f menor me lo salto
        auto entry = Utils::CLOSE.find(currentBoard);
        if(entry != Utils::CLOSE.end()){
            if((entry->g + entry->h) < currentBoard.GetF())
                continue;
        }

        //Check if current board is a win board
        if(Utils::IsWin(currentBoard)){

            Statistics::end = std::chrono::high_resolution_clock::now();

            cout << Utils::FREE  << "Solution found..." << Utils::NORMAL << endl;

            Statistics::totalNodesExpanded = Utils::CLOSE.size();

            if(Utils::showPath)
                ShowMoves(currentBoard, Utils::aStarCache);

            return;
        }

        // Obtener los vecinos del estado actual
        Utils::GetNeighbours(currentBoard, currentBoard.g+1);  
        for (ZenBoard neighbour : Utils::neighbours) {


            //Si está en open o closed y el que habia tiene mejor g,continuar
            //Open es un map de valores g, permite acceder al vlaor ya que la 
            //priority queue no 
            auto entryOpen = Utils::OPEN.find(neighbour);
            if(entryOpen != Utils::OPEN.end()){
                if(entryOpen->second <= neighbour.g)
                    continue;
            }

            auto entryClose = Utils::CLOSE.find(neighbour);
            if(entryClose != Utils::CLOSE.end()){
                if(entryClose->g <= neighbour.g)
                    continue;
            }

            entryClose = Utils::CLOSE.find(neighbour);
            if(entryClose != Utils::CLOSE.end())
                Utils::CLOSE.erase(neighbour);

            //Fast, duplicates?
            openQueue.push(neighbour);

            //Update OPEN
            Utils::OPEN.insert_or_assign(neighbour, neighbour.g);

            //Update path cache
            Utils::aStarCache.insert_or_assign(neighbour, currentBoard);
        }

        Utils::CLOSE.insert(currentBoard);
    }*/

    cout << "Not solution" << endl;
}

//IDASTar
void MyAlgorithms::IDAStar(ZenBoard& zenBoard){

    cout << "-> Empezando IDAStar.." << endl;
    Statistics::start = std::chrono::high_resolution_clock::now();

    ZenBoard* root = &zenBoard;
    root->CompH();
    int bound = zenBoard.h;
    
    Utils::path.push((root)->garden);

    int t = 0;

    while (true) {

        t = Search(root, 0, bound);

        if (t == -1) {
            cout << Utils::FREE << "Solution found..." << Utils::NORMAL << endl;
            break;
        }
        if (t == INT_MAX) {
            cout << Utils::ERROR  << "Not solution..."<< Utils::NORMAL << endl;
            break;
        }
        if(t == INT_MIN){
            cout<<Utils::ERROR << "\nTime out!"<<Utils::NORMAL << endl;
            break;
        }

        bound = t;
        
        //Utils::visited.clear();
    }
}



int MyAlgorithms::Search(ZenBoard* s, int g, int bound) {


    //Check win
    if (Utils::IsWin(*s)){
        Statistics::end = std::chrono::high_resolution_clock::now();
        if(Utils::showPath){

            int cont = 0;
            Utils::PrintBoard(*s);
            cout << "is win" << endl;
            while (!Utils::path.empty()) {
                auto topElement = Utils::path.top();
                cont++;
                Utils::PrintDynamicBitset(topElement);
                Utils::path.pop();
            }
            getchar();

            Statistics::solutionLength = cont-1;
        }

        return -1;
    }
    
    //Get node f
    //El IDA mejorado no tiene esto??
    /*int f = s->GetF();
    if (f > bound) return f;*/
    

    Statistics::totalNodesExpanded++;
    if(Statistics::totalNodesExpanded%1000==0 && !Statistics::isTimeOut){
        Statistics::end = std::chrono::high_resolution_clock::now();
        if( Statistics::IsTimeOut())
        {
            Statistics::end = std::chrono::high_resolution_clock::now();
            Statistics::isTimeOut = true;
            return INT_MIN;
        }
    }

    //Get neighbours
    Utils::GetNeighbours(*s, g+1);
    auto neig = Utils::neighbours;

    //Compute H
    //cout << "Compute H" << endl;
    for (auto it=neig.begin(); it!=neig.end();it++) { 
        auto entry=Utils::TTLookup(*it);

        /*cout << "Entry hashcode: "<<entry._hashcode << endl;
        cout << "(*it)->GetHashCode(): "<<(*it)->GetHashCode()<< endl;
        getchar();*/
        if (entry._hashcode==(*it)->GetHashCode()){
            auto newH=entry._bound;
            if(newH > (*it)->h){
                if(g<bound){
                    bound=g;
                }
            }

            /*
            //Stats??
            if(depth>cutMax){
                    cutMax=depth;
                }
            cutTotal+=depth;
            cutCount++;*/
            
            (*it)->h = newH;
        } 
        //HANS
        //Agregado para calcular el h
        /*else{
            (*it)->CompH();
            //cout << "No entra | H: "<<(*it)->h << endl;
        }*/
    }


    int min = INT_MAX;

    //Nuevo
    for (auto it=neig.begin(); it!=neig.end();it++){

        Utils::path.push((*it)->garden);
        int t;
        if ((*it)->h <= bound) {
			t = 1 + Search(*it, g+1, bound - 1);
		} else {
			t = 1 + (*it)->h;
		}
		delete (*it);
        Utils::path.pop();

        //Time Out
        if(Statistics::isTimeOut==1)
            return INT_MIN;

        //Has a solution
        if (t == -1){
            cout << "Return 1" << endl;
            getchar();
            return -1;
        }
           

        //Update min if is minor than t
        if (t < min) {
            //cout << "t: " << t << " | -> Min: " << min << endl;
			min = t ; // better lower bound
		}
    }

    /*cout << "MIN: " << min << endl;
    getchar();*/
    Utils::TTSave(s->GetHashCode(), min);
    return min;

    //Antiguo
    /*for (auto it=nodesVector.begin(); it!=nodesVector.end();it++) {

        path.push((*it));

        int t = Search(path, (*it)->g, bound);

        if(Statistics::isTimeOut==1)
            return INT_MIN;

        //Has a solution
        if (t == -1)
            return -1;

        //Update min val
        if (t < min)
            min = t; 
        
        delete (*it);
        path.pop();

    }*/


}

//Show moves for A* and BFS
void MyAlgorithms::ShowMoves(ZenBoard zenBoard, unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals>& map){


    cout << Utils::BUSY << "-> Show moves" << Utils::NORMAL<< endl;

    /*stack<ZenBoard> stack;
    ZenBoard currentZenBoard = zenBoard;

    //Push solution
    auto element = map.find(currentZenBoard);
    stack.push(element->first);

    do{
        stack.push(element->second);
        currentZenBoard = map[currentZenBoard];
        element = map.find(currentZenBoard);
    }while(((element->first.player == element->second.player) && 
            (element->first.garden == element->second.garden)) != 1);

    Statistics::solutionLength = stack.size()-1;

    while (!stack.empty()) {
        ZenBoard topElement = stack.top();
        Utils::PrintBoard(topElement);
        stack.pop();
    }*/
}

// Función de comparación para ordenar los nodos según su variable h
bool compareNodes(const ZenBoard* a, const ZenBoard* b) {
    return a->h < b->h;
}

