#ifndef ZEN_UTILS_H
#define ZEN_UTILS_H
#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <fstream>
#include <stack>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <boost/functional/hash.hpp>
#include "ZenBoard.h"
#include "Vector2.h"
#include "Statistics.h"
#include "GameConfig.h"
#include "TTEntry.h"

#define BOOST_DYNAMIC_BITSET_DONT_USE_FRIENDS
using namespace std;

class Utils {

public:
    static int DIMENSION;
    static int showPath;
    static GameConfig gameConfig;

    //Colors
    static const string ERROR;
    static const string FREE;
    static const string BUSY;
    static const string INFO;
    static const string PLAYER;
    static const string NORMAL;
    static const string OTHER;

    //Vectors
    static const Vector2<int> up;
    static const Vector2<int> right;
    static const Vector2<int> down;
    static const Vector2<int> left;

    // Get hashcode from ZenBoard
    struct GetHashCode {
        size_t operator()(const ZenBoard* x) const {
           //std::hash<std::bitset<36>> hash_fn36;
            //return hash_fn36(x->garden) ^ hash_fn36(x->player);
            return operator()(*x);
            }
        size_t operator()(const ZenBoard x) const {
            std::hash<std::bitset<36>> hash_fn36;
            return hash_fn36(x.garden) ^ hash_fn36(x.player);
        }
    };

    //  Get equals value between two ZenBoards
    struct Equals {
        bool operator()(const ZenBoard* x, const ZenBoard* y) const {
            return (x->garden == y->garden) && (x->player == y->player);
        }
        bool operator()(const ZenBoard x, const ZenBoard y) const {
            return (x.garden == y.garden) && (x.player == y.player);
        }
    };


    static unordered_set<ZenBoard*, GetHashCode, Equals> neighbours;
    static unordered_set<ZenBoard*, GetHashCode, Equals> deadlocksTable;

    //TT
    static TTEntry TT[1000000];
    static TTEntry TTLookup(ZenBoard* hashcode);
    static void TTSave(ZenBoard zenBoard, int bound);
    static stack<bitset<36>> path;
    static void PrintTT();

    //A*
    //static unordered_map<ZenBoard, ZenBoard, GetHashCode, Equals> map;
    //static unordered_map<ZenBoard,int, GetHashCode, Equals> OPEN;
    //static unordered_set<ZenBoard, GetHashCode, Equals> CLOSE;
    //static unordered_map<ZenBoard, ZenBoard, GetHashCode, Equals> aStarCache;

    //Transposition table
    

    static void PrintBoard(ZenBoard zenBoard);
    static void PrintBitset(bitset<36> printBitset);    
    static void PrintDynamicBitset(boost::dynamic_bitset<> printBitset);
    static void PrintBoardWIndexs(ZenBoard zenBoard);
    static bool IsWin(ZenBoard& zenBoard);
    static bool IsPlayerOutside(ZenBoard zenBoard);
    static int GetCorrectIndex(int index);
    static int GetCorrectIndex(Vector2<int> entry);
    static int GetMax();
    static void GetNeighbours(ZenBoard zenBoard, int newG);
    static bool IsInside(int index);

    static void DoManualMove(ZenBoard& zenBoard, int currentIndex, Vector2<int> dir);

    static void ManualPaint(ZenBoard& zenBoard, int currentIndex, Vector2<int> direction, int step);
    static void IAPaint(ZenBoard& zenBoard, int currentIndex, Vector2<int> direction, int step);  
    static void PaintChild(const ZenBoard& zenBoard, int currentIndex, Vector2<int> dir, int step, int newG);
    static bool GetEndPaintCondition(Vector2<int> direction, int index, int step, int dirFactor);
    static bool ManualGetEndPaintCondition(Vector2<int> direction, int index);
    static bool CanMove(ZenBoard& zenBoard,int currentIndex, Vector2<int> direction, int step, bool isPlayerInsideBoard);

    static int GetID(ZenBoard zenBoard);
    static Vector2<int> GetDirection(ZenBoard zenBoard);
    static Vector2<int> GetDirectionFromEntry(int id);
    static Vector2<int> GetInitialIndex(int id);
    static int CountSpaces(bitset<36>& gardenClone, int currentIndex, Vector2<int> direction, int step);
    static void GardenPaint(bitset<36>& gardenClone, int currentIndex, int max, int step);  

};

#endif //ZEN_UTILS_H