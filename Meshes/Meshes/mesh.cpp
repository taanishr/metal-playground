//
//  mesh.cpp
//  Meshes
//
//  Created by Taanish Reja on 1/8/25.
//

#include "mesh.h"

MTL::VertexDescriptor* SimpleMesh::defaultVertexDescriptor()
{
    MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
    
    vertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormat::VertexFormatFloat2);
    vertexDescriptor->attributes()->object(0)->setOffset(0);
    vertexDescriptor->attributes()->object(0)->setBufferIndex(0);
    
    vertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormat::VertexFormatFloat4);
    vertexDescriptor->attributes()->object(1)->setOffset(0);
    vertexDescriptor->attributes()->object(1)->setBufferIndex(1);
    
    vertexDescriptor->layouts()->object(0)->setStride(sizeof(simd_float2));
    vertexDescriptor->layouts()->object(1)->setStride(sizeof(simd_float4));
    
    return vertexDescriptor;
}

SimpleMesh::SimpleMesh(int sideCount, float radius, simd_float4 color, MTL::Device* device)
{
    std::vector<simd_float2> positions {};
    std::vector<simd_float4> colors {};
    float angle = std::numbers::pi/2.0f;
    float deltaAngle = (2.0*std::numbers::pi) / float(sideCount);
    for (int i = 0; i <sideCount; ++i) {
        positions.push_back(simd_float2{radius * cos(angle), radius*sin(angle)});
        colors.push_back(color);

        angle += deltaAngle;
    }
    
    positions.push_back(simd_float2{0,0});
    colors.push_back(color);
    
    MTL::Buffer* positionsBuffer = device->newBuffer(positions.data(),
                                                     sizeof(simd_float2)*positions.size(),
                                                     MTL::ResourceStorageModeShared);
    
    MTL::Buffer* colorsBuffer = device->newBuffer(colors.data(),
                                                     sizeof(simd_float4)*colors.size(),
                                                     MTL::ResourceStorageModeShared);
    
    m_vertexBuffers.push_back(positionsBuffer);
    m_vertexBuffers.push_back(colorsBuffer);
    
    m_vertexCount = positions.size();
    
    std::vector<uint16_t> indices;
    uint16_t count = sideCount;
    
    for (int i = 0; i < count; ++i) {
        indices.push_back(i);
        indices.push_back(count);
        indices.push_back((i+1) % count);
    }
    
    MTL::Buffer* indexBuffer = device->newBuffer(indices.data(), sizeof(uint16_t)*indices.size(), MTL::StorageModeShared);
    
    m_indexBuffer = indexBuffer;
    
    m_indexCount = indices.size();
    
    m_vertexDescriptor = defaultVertexDescriptor();
}

SimpleMesh::~SimpleMesh()
{
    for (MTL::Buffer* vertexBuffer : m_vertexBuffers) {
        if (vertexBuffer)
            vertexBuffer->release();
    }
    
    if (m_indexBuffer)
        m_indexBuffer->release();
    
    m_vertexDescriptor->release();
}
