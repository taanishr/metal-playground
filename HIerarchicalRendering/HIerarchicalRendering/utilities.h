//
//  utilities.h
//  FunWithConstants
//
//  Created by Taanish Reja on 1/8/25.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include "simd_utilities.h"

constexpr int align(int size, int upTo)
{
    return (size + upTo - 1) & ~(upTo - 1);
}

#endif
