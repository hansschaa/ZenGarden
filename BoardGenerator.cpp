#include <algorithm>
#include "BoardGenerator.h"


int BoardGenerator::mu = 12;
int BoardGenerator::lambda = 10;
int BoardGenerator::iterations = 50;

// Definición de la función de comparación
bool compareByFitness(const MyIndividual& a, const MyIndividual& b) {
    return a.fitness < b.fitness;
};

// Definición de la función de comparación
bool betters(const MyIndividual& a, const MyIndividual& b) {
    return a.fitness > b.fitness;
};


void BoardGenerator::Run()
{
    cout << "-----> Run" << endl;

    vector<MyIndividual> population = GetInitialPopulation();
    bool mutationSuccess = false;
    int cont = 0;
    while(cont < iterations){
        cout << "-----> Iteration: " << cont << endl;
        // Llamada a std::sort
        std::sort(population.begin(), population.end(), compareByFitness);
        // Eliminar los primeros 5 elementos
        population.erase(population.begin(), population.begin() + mu);

        for (int i = 0; population.size() != (mu+lambda); i++) {
            MyIndividual duplicate;
            duplicate.zenBoard.garden = population[i].zenBoard.garden; 
        
            duplicate.fitness = population[i].fitness; 

            do{
                mutationSuccess = duplicate.Mutate();
            }while(!mutationSuccess);

            population.push_back(duplicate);

            if(i == lambda-1)
                i =0;
        }

        cont++;
    }

    //Results
    std::sort(population.begin(), population.end(), betters);

    cont = 1;
    // Recorriendo el vector
    for (MyIndividual bitsetElement : population) {
        cout << "Index: " << cont << "| Fitness: " << bitsetElement.fitness << endl;
        Utils::PrintBoard(bitsetElement.zenBoard);
        cont++;
    }

    cout << "fin" << endl;
}

vector<MyIndividual> BoardGenerator::GetInitialPopulation()
{
    srand (time(NULL));
    
    vector<MyIndividual> initialPopulation;
    unordered_set<ZenBoard, Utils::GetHashCode, Utils::Equals> stored;

    MyIndividual myIndividual;
    std::unordered_set<ZenBoard, Utils::GetHashCode, Utils::Equals>::iterator it;
    bitset<36> individual("000000000000000000000000000000000000");
    int fitness = 0;
    for(int i = 0 ; i < lambda+mu ; i++){
        do{
            individual.reset();
            myIndividual.zenBoard.Reset();

            int randomIndex = rand() % 36;
            individual.flip(randomIndex);
            myIndividual.zenBoard.garden = bitset<36> (individual);
            myIndividual.fitness = 0;
            it = stored.find(myIndividual.zenBoard);

            fitness = MyAlgorithms::AStar(myIndividual);

        }while(fitness == -1 || it != stored.end());

        myIndividual.fitness = fitness;
        stored.insert(myIndividual.zenBoard);
        initialPopulation.push_back(myIndividual);
        cout << "Lenght: " << initialPopulation.size() << endl;
    }

    return initialPopulation;
}
