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

class Node {
public:
    Node() = default;
    
    Node(ModelIO::MTKMesh mesh);
    
    void addChildNode(Node* node);
    void removeFromParent();
    void removeChildNode(Node* node);
    simd_float4x4 worldTransform() const;
    
    ModelIO::MTKMesh m_mesh;
    NS::SharedPtr<MTL::Texture> m_texture;
    simd_float4x4 m_transform = matrix_identity_float4x4;
    std::vector<Node*> m_nodes;
    Node* m_parentNode;
};
#endif
