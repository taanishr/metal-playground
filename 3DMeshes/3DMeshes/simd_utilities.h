//
//  simd_utilities.h
//  2DMath
//
//  Created by Taanish Reja on 1/8/25.
//

#ifndef SIMD_UTILITIES_H
#define SIMD_UTILITIES_H
namespace simdHelpers {
    simd_float4x4 scale2D(simd_float2 s);
    simd_float4x4 rotateZ(float zRadians);
    simd_float4x4 translate2D(simd_float2 t);
    simd_float4x4 orthogonicProjection(float left, float top,
                                       float right, float bottom,
                                       float near, float far);
}
#endif
