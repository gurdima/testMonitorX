#pragma once

#include <cstddef>

template<typename T>
inline T interpolate(T *args,T *vals, size_t sz, const T& x)
{
    int k = 0;
    size_t intervalsNumber = sz - 1;
    int pos[2] = {0, intervalsNumber};
    int temp;
    while ((pos[1] - pos[0]) > 0) {
        k = (pos[0] + pos[1]) >> 1;

        if ( (args[k] <= x) && (args[k + 1] >= x) ) {
            break;
        }

        temp = ((args[k] - x) >= 0);
        pos[temp] = k + (-1 * temp + 1);
    }

    if (pos[0] != pos[1]) {
        T k1 = (vals[k + 1] - vals[k]) / (args[k + 1] - args[k]);
        return vals[k] + (k1 * (x - args[k]));
    } else {
        return vals[intervalsNumber * (1 - (pos[0] == 0))];
    }
}

// #endif // INTERPOLATE_H
