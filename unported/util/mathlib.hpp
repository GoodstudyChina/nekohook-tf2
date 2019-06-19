
/*
 *
 *	Helpers for math conversion between the framework and the module
 *
 */

#pragma once
#include "../../../util/mathlib.hpp"  // CatVector()
#include "../sdk/sdk.hpp"             // Vector()

inline CatVector ToCatVector(const Vector& input) {
    return CatVector(input.x, input.y, input.z);
}

inline Vector ToVector(const CatVector& input) {
    return Vector(input.x, input.y, input.z);
}

inline CatVector VectorAngles(const CatVector& forward) {
    CatVector ret;
    if (forward.y == 0 && forward.x == 0) {
        if (forward.z > 0)
            ret.x = 270;
        else
            ret.x = 90;
    } else {
        ret.y = atan2(forward.y, forward.x) * 180 / M_PI;
        if (ret.y < 0) ret.y += 360;
        ret.x = atan2(-forward.z,
                      sqrt(forward.x * forward.x + forward.y * forward.y)) *
                180 / M_PI;
        if (ret.x < 0) ret.x += 360;
    }
    return ret;
}
