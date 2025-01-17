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

    simd_float4x4 scale(simd_float3 s)
    {
        return {
            simd_float4{s.x,0,0,0},
            simd_float4{0,s.y,0,0},
            simd_float4{0,0,s.z,0},
            simd_float4{0,0,0,1.0}
        };
    }

    simd_float4x4 rotateAbout(simd_float3 axis, float radians)
    {
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;
        float s = sin(radians);
        float c = cos(radians);

        return {
            simd_float4{x * x + (1 - x * x) * c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0},
            simd_float4{x * y * (1 - c) + z * s, y * y + (1 - y * y) * c, y * z * (1 - c) - x * s, 0},
            simd_float4{x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z + (1 - z * z) * c, 0},
            simd_float4{0,0,0, 1}
        };
    }

    simd_float4x4 translate(simd_float3 t)
    {
        return {
            simd_float4{1.0,0,0,0},
            simd_float4{0,1.0,0,0},
            simd_float4{0,0,1.0,0},
            simd_float4{t.x,t.y,t.z,1.0}
        };
    }

    simd_float4x4 perspectiveProjection(float fovYRadians, float aspectRatio, float near, float far)
    {
        float sy = 1 / tan(fovYRadians * 0.5);
        float sx = sy / aspectRatio;
        float zRange = far - near;
        float sz = -(far + near) / zRange;
        float tz = -2 * far * near / zRange;
        return {simd_float4{sx, 0,  0,  0},
            simd_float4{0, sy,  0,  0},
            simd_float4{0,  0, sz, -1},
            simd_float4{0,  0, tz,  0}
        };
    }
}
