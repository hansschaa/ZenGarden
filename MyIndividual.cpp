#include <time.h>
#include <iostream>
#include <random>
#include "MyIndividual.h"
#include "MyAlgorithms.h"

bool MyIndividual::Mutate()
{
    srand (time(NULL));
    int percent = (rand() % 100);

    if(percent <= moveMutation)
        return MoveMutation();

    else if(percent > moveMutation && percent <= 85)
        return AddMutation();

    else
        return RemoveMutation();
}

bool MyIndividual::MoveMutation()
{
    if(!zenBoard.garden.any())
        return false;

    //Copy
    bitset<36> copy = zenBoard.garden;

    //Count
    int count = copy.count();

    //Random id
    int random = (rand() % count);

    //Fill 1s index list
    int auxIndex;
    int list[count];
    int cont = 0;
    while(copy.any()){
        auxIndex = copy._Find_first();
        list[cont] = auxIndex;
        copy.set(auxIndex, 0);
        cont++;
    }

    //Recopy
    copy = zenBoard.garden;
    //Old
    int oldIndex = list[random];
    //New
    int newIndex;
    do{
        newIndex = (rand() % 36);
    }while(copy[newIndex] == 1);

    //Swap
    copy.set(oldIndex, 0);
    copy.set(newIndex, 1);

     //Try resolve
    MyIndividual myAuxIndividual;
    myAuxIndividual.zenBoard.garden = copy;
    int newFitness = MyAlgorithms::AStar(myAuxIndividual);
    if(newFitness != -1){
        //Update
        zenBoard.garden = copy;
        fitness =  newFitness;

        return true;
    }

     return false;
}

bool MyIndividual::AddMutation()
{
    if(zenBoard.garden.count() > maxRocks)
        return false;

    //Copy
    bitset<36> copy = zenBoard.garden;

    int newIndex;
    do{
        newIndex = (rand() % 36);
    }while(copy[newIndex] == 1);

    copy.set(newIndex, 1);

     //Try resolve
    MyIndividual myAuxIndividual;
    myAuxIndividual.zenBoard.garden = copy;
    int newFitness = MyAlgorithms::AStar(myAuxIndividual);
    if(newFitness != -1){
        //Update
        zenBoard.garden = copy;
        fitness =  newFitness;

        return true;
    }

     return false;
}

bool MyIndividual::RemoveMutation()
{
     if(!zenBoard.garden.any())
        return false;

    //Copy
    bitset<36> copy = zenBoard.garden;

    //Count
    int count = copy.count();

    //Random id
    int random = (rand() % count);

    //Fill 1s index list
    int auxIndex;
    int list[count];
    int cont = 0;
    while(copy.any()){
        auxIndex = copy._Find_first();
        list[cont] = auxIndex;
        copy.set(auxIndex, 0);
        cont++;
    }

    copy.set(list[random], 0);
    
    //Try resolve
    MyIndividual myAuxIndividual;
    myAuxIndividual.zenBoard.garden = copy;
    int newFitness = MyAlgorithms::AStar(myAuxIndividual);
    if(newFitness != -1){
        //Update
        zenBoard.garden = copy;
        fitness =  newFitness;

        return true;
    }

     return false;
}