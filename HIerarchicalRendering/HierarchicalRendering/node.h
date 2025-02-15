//
//  node.h
//  HIerarchicalRendering
//
//  Created by Taanish Reja on 1/16/25.
//

#ifndef NODE_H
#define NODE_H
#include "metal_imports.h"
#include "ModelIO.h"

class NodeConstants {
public:
    NodeConstants(simd_float4x4 modelViewProjectionMatrix, simd_float4 color);
    simd_float4x4 m_modelViewProjectionMatrix;
    simd_float4 m_color;
};

class Node {
public:
    Node() = default;
    
    Node(ModelIO::MTKMesh mesh);
    
    void addChildNode(Node* node);
    void removeFromParent();
    void removeChildNode(Node* node);
    simd_float4x4 worldTransform() const;
    
    ModelIO::MTKMesh m_mesh;
    simd_float4 m_color = simd_float4{1,1,1,1};
    simd_float4x4 m_transform = matrix_identity_float4x4;
    std::vector<Node*> m_nodes;
    Node* m_parentNode;
};
#endif
