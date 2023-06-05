#include <unordered_set>
#include <iostream>
#include <chrono>
#include <stack>
#include <limits.h>
#include "Statistics.h"
#include "MyAlgorithms.h"
#include "Utils.h"

using namespace std;

void MyAlgorithms::BFS(ZenBoard& zenBoard, int showPath){
    
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
            if(showPath)
                ShowMoves(currentBoard);
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

void MyAlgorithms::AStar(ZenBoard& zenBoard, int showPath){
    
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
    zenBoard.h = zenBoard.CompH();

    //For parents
    Utils::map.insert({zenBoard, zenBoard});  
    //Insert root node in open priority queue
    openQueue.push(zenBoard);
   

    while(!openQueue.empty()){

        //Get the min value in priority queue
        auto currentBoard = openQueue.top();
		openQueue.pop();

        // Si el nodo estaba en la lista cerrada con un f menor me lo salto
        auto entry = Utils::CLOSE.find(currentBoard);
        if(entry != Utils::OPEN.end()){
            if((entry->g+entry->h) < currentBoard.GetF())
                continue;
        }

        //Check if current board is a win board
        if(Utils::IsWin(currentBoard)){

            Statistics::end = std::chrono::high_resolution_clock::now();

            cout << "Solution founded...asasa"<< endl;

            Statistics::totalNodesExpanded = Utils::CLOSE.size();
            if(showPath)
                ShowMoves(currentBoard);
        }

        Utils::CLOSE.insert(currentBoard);

        // Obtener los vecinos del estado actual
        Utils::GetNeighbours(currentBoard);  
        for (ZenBoard neigbour : Utils::neighbours) {

            //Considerer
            int newG = currentBoard.g+1;

            //Si está en open o closed y el que habia tiene mejor g,continuar
            auto entry = Utils::OPEN.find(neigbour);
            if(entry != Utils::OPEN.end()){
                if(entry->g <= newG){
                    continue;
                }
            }

            entry = Utils::CLOSE.find(neigbour);
            if(entry != Utils::CLOSE.end()){
                if(entry->g <= newG){
                    continue;
                }
            }

            neigbour.g = newG;
            neigbour.h = neigbour.CompH();

            //Remove from close
            if(entry != Utils::CLOSE.end()){
                //Reopen 
                Utils::CLOSE.erase(neigbour);
            }

            //Fast, duplicates?
            openQueue.push(neigbour);

            /*entry = Utils::OPEN.find(neigbour);
            if(entry != Utils::OPEN.end()){
                ZenBoard openZenBoard = *entry;
                openZenBoard.g = neigbour.g;
                Utils::OPEN.erase(neigbour);
                Utils::OPEN.insert(openZenBoard);
            }
            else
                Utils::OPEN.insert(neigbour);*/

        }
    }
}

void MyAlgorithms::IDAStar(ZenBoard& zenBoard, int showPath){
    
    //int maxDeph = 100;
    zenBoard.CompH();
    int bound = zenBoard.h;
    std::vector<ZenBoard> path;
    path.push_back(zenBoard);
    int t;

    while (true) {
        //TODO
        //dfs
        if (t == -1) {
            cout << "Solution founded..." << endl;
            break;
        }
        if (t == INT_MAX) {
            cout << "Not solution..." << endl;
            break;
        }
        bound = t;
    }
}

void MyAlgorithms::ShowMoves(ZenBoard zenBoard){

    cout << Utils::ERROR << "-> Show moves" << Utils::NORMAL<< endl;

    stack<ZenBoard> stack;
    ZenBoard currentZenBoard = zenBoard;
   
    while(Utils::map[currentZenBoard].garden != currentZenBoard.garden){
        stack.push(currentZenBoard);
        currentZenBoard = Utils::map[currentZenBoard];
    }

    stack.push(currentZenBoard);

    Statistics::solutionLength = stack.size();

    while (!stack.empty()) {
        ZenBoard topElement = stack.top();
        Utils::PrintBoard(topElement);
        stack.pop();
    }

}