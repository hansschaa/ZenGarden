#include "BoardGenerator.h"

int BoardGenerator::mu = 10;
int BoardGenerator::lambda = 10;
int BoardGenerator::iterations = 100;

//Templates
// bitset<36> pointTemplate("100000000000000000000000000000000000"); 
// bitset<36> twopointHorizontalTemplate("110000000000000000000000000000000000"); 
// bitset<36> twopointVerticalTemplate("100000100000000000000000000000000000");

void BoardGenerator::Run()
{
    cout << "Run" << endl;

    vector<std::bitset<36>> initialPopultation = GetInitialPopulation();

    // Recorriendo el vector
    for (const std::bitset<36>& bitsetElement : initialPopultation) {
        Utils::PrintBitset(bitsetElement);
    }

    int cont = 0;
    while(cont < iterations){

        
        
        cont++;

    }
    cout << "fin" << endl;

}

vector<std::bitset<36>> BoardGenerator::GetInitialPopulation()
{
    srand (time(NULL));
    
    vector<std::bitset<36>> initialPopulation;
    unordered_set<ZenBoard, Utils::GetHashCode, Utils::Equals> stored;

    ZenBoard zenBoard;
    std::unordered_set<ZenBoard, Utils::GetHashCode, Utils::Equals>::iterator it;
    bitset<36> individual("000000000000000000000000000000000000");

    for(int i = 0 ; i < lambda+mu ; i++){
        do{
            individual.reset();
            zenBoard.Reset();
            int randomIndex = rand() % 36;
            individual.flip(randomIndex);
            zenBoard.garden = bitset<36> (individual);
            it = stored.find(zenBoard);

        }while(!MyAlgorithms::AStar(zenBoard) || it != stored.end());

        stored.insert(zenBoard);
        initialPopulation.push_back(individual);
        cout << "Lenght: " << initialPopulation.size() << endl;
    }

    return initialPopulation;
}
