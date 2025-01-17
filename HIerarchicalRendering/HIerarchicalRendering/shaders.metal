//
//  shaders.metal
//  Drawing2DShapes
//
//  Created by Taanish Reja on 1/8/25.
//

#include <metal_stdlib>
using namespace metal;

struct NodeConstants {
    float4x4 modelViewProjectionMatrix;
    float4 color;
};


struct VertexIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
};
struct VertexOut {
    float4 position [[position]];
    float3 normal;
    float4 color;
};

vertex VertexOut vertex_main(
    VertexIn in [[stage_in]],
    constant NodeConstants &constants [[buffer(2)]])
{
    VertexOut out;
    out.position = constants.modelViewProjectionMatrix * float4(in.position, 1.0);
    out.normal = in.normal;
    out.color = constants.color;
    return out;
}

fragment float4 fragment_main(VertexOut in [[stage_in]]) {
    float3 N = normalize(in.normal);
    float4 color = in.color;
    return color;
}
