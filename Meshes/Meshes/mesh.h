//
//  mesh.h
//  Meshes
//
//  Created by Taanish Reja on 1/8/25.
//
#ifndef MESH_H
#define MESH_H
#include "metal_imports.h"
#include <vector>
#include <numbers>

class SimpleMesh {
public:
    static MTL::VertexDescriptor* defaultVertexDescriptor();
    SimpleMesh(int sideCount, float radius, simd_float4 color, MTL::Device* device);
    ~SimpleMesh();
    std::vector<MTL::Buffer*> m_vertexBuffers;
    MTL::VertexDescriptor* m_vertexDescriptor;
    int m_vertexCount;
    MTL::PrimitiveType m_primitiveType = MTL::PrimitiveType::PrimitiveTypeTriangle;
    MTL::Buffer* m_indexBuffer;
    MTL::IndexType m_indexType = MTL::IndexType::IndexTypeUInt16;
    int m_indexCount;
};

#endif
