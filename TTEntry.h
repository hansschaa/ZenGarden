#ifndef ZEN_TTENTRY_H
#define ZEN_TTENTRY_H
#include <stdlib.h>
using namespace std;

class TTEntry {

public:
    int _bound;
    size_t _hashcode;

    TTEntry(){
        _hashcode = -1;
    }

    TTEntry(size_t hashcode, int bound){
        _hashcode = hashcode;
        _bound = bound;
    }
};

#endif //ZEN_TTENTRY_H