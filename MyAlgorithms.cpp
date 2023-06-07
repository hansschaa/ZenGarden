#include <unordered_set>
#include <iostream>
#include <chrono>
#include <stack>
#include <limits.h>
#include "Statistics.h"
#include "MyAlgorithms.h"
#include "Utils.h"

using namespace std;

void MyAlgorithms::BFS(ZenBoard& zenBoard){
    
    cout << "-> Empezando BFS" << endl;
    Statistics::start = std::chrono::high_resolution_clock::now();
    queue<ZenBoard> queue;     
    queue.push(zenBoard);                 
    Utils::visited.insert(zenBoard); 
    Utils::map.insert({zenBoard, zenBoard});    

    while (!queue.empty()) {

        ZenBoard currentBoard = queue.front();
        queue.pop();

        if (Utils::IsWin(currentBoard)) {
            Statistics::end = std::chrono::high_resolution_clock::now();
            cout << "Solution founded..."<< endl;
            if(Utils::showPath){
                //Utils::PrintBoard(currentBoard);
                //Utils::PrintBoard(Utils::map[currentBoard]);
                //Utils::PrintBoard(Utils::map[Utils::map[currentBoard]]);
                ShowMoves(currentBoard, Utils::map);
            }
               
            break;
        }

        // Obtener los vecinos del estado actual
        Utils::GetNeighbours(currentBoard);  
        for (ZenBoard neigbord : Utils::neighbours) {

            // Si el vecino no ha sido visitado anteriormente
            if (Utils::visited.find(neigbord) == Utils::visited.end()) {
                queue.push(neigbord);                               
                Utils::visited.insert(neigbord);      
            }
        }

        Statistics::totalNodesExpanded++;
    }
}

void MyAlgorithms::AStar(ZenBoard& zenBoard){
    
    cout << "-> Empezando AStar.." << endl;
    Statistics::start = std::chrono::high_resolution_clock::now();

    //Sort ascendent
    struct ZenBoardComparator {
        bool operator()(const ZenBoard& zenBoard1, const ZenBoard& zenBoard2) const {

            // Orden ascendente basado en la variable f
            return zenBoard1.g + zenBoard1.h < zenBoard2.g + zenBoard2.h;
            //return false;
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

            cout << "Solution founded..."<< endl;

            Statistics::totalNodesExpanded = Utils::CLOSE.size();

            if(Utils::showPath)
                ShowMoves(currentBoard, Utils::aStarCache);

            return;
        }

        // Obtener los vecinos del estado actual
        Utils::GetNeighbours(currentBoard);  
        for (ZenBoard neigbour : Utils::neighbours) {

            //Considerer
            int newG = currentBoard.g+1;

            //Si está en open o closed y el que habia tiene mejor g,continuar
            //Open es un map de valores g, permite acceder al vlaor ya que la 
            //priority queue no 
            auto entryOpen = Utils::OPEN.find(neigbour);
            if(entryOpen != Utils::OPEN.end()){
                if(entryOpen->second <= newG)
                    continue;
            }

            auto entryClose = Utils::CLOSE.find(neigbour);
            if(entryClose != Utils::CLOSE.end()){
                if(entryClose->g <= newG)
                    continue;
            }

            neigbour.g = newG;
            neigbour.CompH();

            entryClose = Utils::CLOSE.find(neigbour);
            if(entryClose != Utils::CLOSE.end())
                Utils::CLOSE.erase(neigbour);

            //Fast, duplicates?
            openQueue.push(neigbour);

            //Update OPEN
            Utils::OPEN.insert_or_assign(neigbour, newG);

            //Update path cache
            Utils::aStarCache.insert_or_assign(neigbour, currentBoard);
        }

        Utils::CLOSE.insert(currentBoard);
    }
}

void MyAlgorithms::IDAStar(ZenBoard& zenBoard){

    cout << "-> Empezando IDAStar.." << endl;
    Statistics::start = std::chrono::high_resolution_clock::now();

    zenBoard.CompH();
    int bound = zenBoard.h;
    cout << "Bound inicial " << bound <<endl;
    deque<ZenBoard> path;
    path.push_front(zenBoard);
    Utils::map.insert({zenBoard, zenBoard}); 

    int t = 0;

    while (true) {

        t = Search(path, 0, bound);

        if (t == -1) {
            cout << "Solution found..." << endl;
            break;
        }
        if (t == INT_MAX) {
            cout << "Not solution..." << endl;
            break;
        }
        bound = t;
    }
}

int MyAlgorithms::Search(deque<ZenBoard>& path, int g, int bound) {

    //Get last node in path
    ZenBoard node = path.back(); 

    //Compute h node
    node.CompH();
    int f = node.GetF();
    if (f > bound)
        return f;
    if (Utils::IsWin(node)){
       
        Statistics::end = std::chrono::high_resolution_clock::now();
        if(Utils::showPath)
            ShowMoves(node, Utils::map);
        return -1;
    }
        
    int min = INT_MAX;

    Utils::GetNeighbours(node);

    for (ZenBoard neighbour : Utils::neighbours) {
        if (find(path.begin(), path.end(), neighbour) == path.end()) {
            
            path.push_back(neighbour);
            int t = Search(path, g + 1, bound);
            path.pop_back();

            if (t == -1)
                return -1;

            //Update min val
            if (t < min)
                min = t; 
        }
    }

    Utils::PrintBoard(node);
    return min;
}

void MyAlgorithms::ShowMoves(ZenBoard zenBoard, unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals>& map){

    cout << Utils::ERROR << "-> Show moves" << Utils::NORMAL<< endl;

    stack<ZenBoard> stack;
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

    Statistics::solutionLength = stack.size();

    while (!stack.empty()) {
        ZenBoard topElement = stack.top();
        Utils::PrintBoard(topElement);
        stack.pop();
    }
}


