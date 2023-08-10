#ifndef ZEN_TTENTRY_H
#define ZEN_TTENTRY_H
#include <stdlib.h>
using namespace std;

class TTEntry {

public:
    ZenBoard _zenBoard;
    int _bound;
    
    TTEntry(){
        _bound = -1;
    }

    TTEntry(ZenBoard zenBoard, int bound): _zenBoard(zenBoard), _bound(bound){
    }
};

#endif //ZEN_TTENTRY_H