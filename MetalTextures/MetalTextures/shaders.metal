//
//  shaders.metal
//  Drawing2DShapes
//
//  Created by Taanish Reja on 1/8/25.
//

#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float2 texCords [[attribute(2)]];
};
struct VertexOut {
    float4 position [[position]];
    float3 normal;
    float2 texCords;
};

vertex VertexOut vertex_main(
    VertexIn in [[stage_in]],
    constant float4x4 &transform [[buffer(2)]])
{
    VertexOut out;
    out.position = transform * float4(in.position, 1.0);
    out.normal = in.normal;
    out.texCords = in.texCords;
    return out;
}

fragment float4 fragment_main(
    VertexOut in [[stage_in]],
    texture2d<float, access::sample> textureMap [[texture(0)]],
    sampler textureSampler [[sampler(0)]]
) {
    float4 color = textureMap.sample(textureSampler, in.texCords);
    return color;
}
