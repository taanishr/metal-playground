//
//  simd_utilities.cpp
//  2DMath
//
//  Created by Taanish Reja on 1/8/25.
//

#include "metal_imports.h"
#include "utilities.h"

namespace simdHelpers {
    simd_float4x4 scale2D(simd_float2 s)
    {
        return {
            simd_float4{s.x, 0, 0, 0},
            simd_float4{0, s.y, 0, 0},
            simd_float4{0, 0, 1.0, 0},
            simd_float4{0, 0, 0, 1.0}
        };
    }

    simd_float4x4 rotateZ(float zRadians)
    {
        float s = sin(zRadians);
        float c = cos(zRadians);
        
        return {
            simd_float4{c, s, 0, 0},
            simd_float4{-s, c, 0, 0},
            simd_float4{0, 0, 1.0, 0},
            simd_float4{0, 0, 0, 1.0}
        };
    }

    simd_float4x4 translate2D(simd_float2 t)
    {
        return {
            simd_float4{1.0, 0, 0, 0},
            simd_float4{0, 1.0, 0, 0},
            simd_float4{0, 0, 1.0, 0},
            simd_float4{t.x, t.y, 0, 1.0}
        };
    }

    simd_float4x4 orthogonicProjection(float left, float top,
                                       float right, float bottom,
                                       float near, float far)
    {
        float sx = 2 / (right - left);
        float sy = 2 / (top - bottom);
        float sz = 1 / (near - far);
        float tx = (left + right) / (left - right);
        float ty = (top + bottom) / (bottom - top);
        float tz = near / (near - far);
        
        return {
            simd_float4{sx, 0, 0, 0},
            simd_float4{0, sy, 0, 0},
            simd_float4{0, 0, sz, 0},
            simd_float4{tx, ty, tz, 1.0}
        };
    }
}
