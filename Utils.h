#pragma once
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class Vector2
{
public:
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Vector2& obj);
    float x;
    float y;
};

float RelativeRNG(float n, int n_0=20, int n_1=80);