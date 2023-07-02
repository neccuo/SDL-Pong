#include "Utils.h"

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

std::ostream& operator<<(std::ostream& os, const Vector2& v2)
{
    os << "(" << v2.x << ", " << v2.y << ")";
    return os;
}

float RelativeRNG(float n, int n_0, int n_1)
{
    float num = ((rand() % (n_1-n_0)) + n_0) / 100.0f;
    int sign = n < 0 ? -1 : 1;

    return sign * num;
}