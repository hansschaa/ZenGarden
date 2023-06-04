#ifndef ZEN_VECTOR2_H
#define ZEN_VECTOR2_H

#include <iostream>
using namespace std;

template <class Num_T>
class Vector2 {
public:
    Vector2(Num_T x, Num_T y);
    Vector2<int> GetDirection(Vector2<int> other){
        Vector2<int> diff = Vector2<int> (i - other.i, j - other.j);
        if(diff.i > 0) diff.i = 1;
        else if(diff.i < 0) diff.i = -1;
        if(diff.j > 0) diff.j = 1;
        else if(diff.j < 0) diff.j = -1;

        return diff;
    }

    

    Vector2 operator+(const Vector2 &rhs) const;
    Vector2 operator-(const Vector2 &rhs) const;
    Vector2 operator*(Num_T value) const;

    Num_T i;
    Num_T j;

    void Print(){
        cout << i << "," << j << endl;
    }
};


template <class Num_T>
Vector2<Num_T>::Vector2(Num_T x, Num_T y)
        : i(x), j(y) {
}

template <class Num_T>
Vector2<Num_T> Vector2<Num_T>::operator+(const Vector2 &rhs) const {
    return {i + rhs.i, j + rhs.j};
}

template <class Num_T>
Vector2<Num_T> Vector2<Num_T>::operator-(const Vector2 &rhs) const {
    return {i - rhs.i, j - rhs.j};
}
template <class Num_T>
Vector2<Num_T> Vector2<Num_T>::operator*(Num_T value) const {
    return {i * value, j * value};
}

#endif //ZEN_VECTOR2_H


