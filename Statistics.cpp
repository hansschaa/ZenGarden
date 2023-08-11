#include <iostream>
#include <chrono>
#include "Statistics.h"
#include "Utils.h"

using namespace std;

int Statistics::turn = 0;
int Statistics::solutionLength = 0;
int Statistics::totalNodesExpanded = 0;
int Statistics::isTimeOut = 0;
int Statistics::maxMilliseconds = 0;
int Statistics::alleys = 0;
int Statistics::corners = 0;

std::chrono::high_resolution_clock::time_point Statistics::start;
std::chrono::high_resolution_clock::time_point Statistics::end;

std::chrono::high_resolution_clock::time_point Statistics::h_start;
double Statistics::h_total = 0;

std::chrono::high_resolution_clock::time_point Statistics::gen_start;
double Statistics::gen_total = 0;


//Print search stats
void Statistics::Print(string algorithmName){
    cout << Utils::INFO << endl;

    cout<<"ALGORITHM: "<< algorithmName<<endl;
    cout<<"Alleys: "<< alleys << endl;
    cout<<"CORNERS: "<< corners << endl;
    cout<<"VISITED: "<<totalNodesExpanded<< endl;
    cout<<"H COMPUTE in "<< h_total <<"ms" << endl;
    cout<<"GENERATE CHILDS in "<< gen_total <<"ms" << endl;
    cout<<"FOUND in "<<((end-start).count()/1000000.0)<<"ms" << endl;
     cout<<"LENGTH: "<<solutionLength<< endl;
   
    /*cout << corners << "," << alleys << ",0," << totalNodesExpanded 
    << "," << h_total << "," << gen_total << "," << ((end-start).count()/1000000.0) << endl;*/
}

bool Statistics::IsTimeOut(){
    return ((Statistics::end-Statistics::start).count()/1000000.0) > Statistics::maxMilliseconds;
}