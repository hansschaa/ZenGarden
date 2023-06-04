#ifndef ZEN_UTILS_H
#define ZEN_UTILS_H
#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <fstream>
#include "ZenBoard.h"
#include "Vector2.h"

using namespace std;

class Utils {

public:
    static const int DIMENSION = 6;

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
        size_t operator()(const ZenBoard& x) const {

            std::hash<std::bitset<36>> hash_fn81;
            unsigned long hash;

            hash = hash_fn81(x.garden);

            return hash;
        }
    };

    //  Get equals value between two ZenBoards
    struct Equals {
        bool operator()(const ZenBoard& x, const ZenBoard& y) const {

            bool areEquals = (x.garden &~ y.garden).none();

            return areEquals;
        }
    };

    static unordered_set<ZenBoard, GetHashCode, Equals> neighbours;
    static unordered_set<ZenBoard, GetHashCode, Equals> visited;
    static unordered_map<ZenBoard, ZenBoard, GetHashCode, Equals> map;

    static unordered_set<ZenBoard, Utils::GetHashCode,  Utils::Equals> OPEN;
    static unordered_set<ZenBoard, Utils::GetHashCode,  Utils::Equals> CLOSE;

    static void PrintBoard(ZenBoard zenBoard);
    static void PrintBoardWIndexs(ZenBoard zenBoard);
    static bool IsWin(ZenBoard& zenBoard);
    static bool IsPlayerOutside(ZenBoard zenBoard);
    static int GetCorrectIndex(int index);
    static int GetCorrectIndex(Vector2<int> entry);
    static int GetMax();
    static void GetNeighbours(ZenBoard zenBoard);
    static bool IsInside(int index);

    static void DoManualMove(ZenBoard& zenBoard, int currentIndex, Vector2<int> dir);

    static void ManualPaint(ZenBoard& zenBoard, int currentIndex, Vector2<int> direction, int step);
    static void IAPaint(ZenBoard& zenBoard, int currentIndex, Vector2<int> direction, int step);  
    static bool GetEndPaintCondition(Vector2<int> direction, int index, int step, int dirFactor);
    static bool ManualGetEndPaintCondition(Vector2<int> direction, int index);
    static bool CanMove(ZenBoard& zenBoard,int currentIndex, Vector2<int> direction, int step, bool isPlayerInsideBoard);

    static int GetID(ZenBoard zenBoard);
    static Vector2<int> GetDirection(ZenBoard zenBoard);
    static Vector2<int> GetDirectionFromEntry(int id);
    static Vector2<int> GetInitialIndex(int id);

};

#endif //ZEN_UTILS_H