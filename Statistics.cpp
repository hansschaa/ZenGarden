#include <iostream>
#include <chrono>
#include "Statistics.h"
#include "Utils.h"

using namespace std;

int Statistics::turn = 0;
int Statistics::solutionLength = 0;
int Statistics::totalNodesExpanded = 0;
std::chrono::high_resolution_clock::time_point Statistics::start;
std::chrono::high_resolution_clock::time_point Statistics::end;

void Statistics::Print(){
    cout << Utils::INFO << endl;

    cout<<"VISITED: "<<totalNodesExpanded<< endl;
    cout<<"LENGTH: "<<solutionLength<< endl;
    cout<<"FOUND in "<<((end-start).count()/1000000.0)<<"ms" << endl;
}