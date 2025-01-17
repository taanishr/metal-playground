//
//  node.cpp
//  HIerarchicalRendering
//
//  Created by Taanish Reja on 1/16/25.
//

#include "node.h"

class Node {
public:
    ModelIO::MTKMesh m_mesh;
    simd_float4 m_color = simd_float4{1,1,1,1};
    simd_float4x4 m_transform = matrix_identity_float4x4;
    std::vector<Node*> m_nodes;
    Node* m_parentNode;
    
    void addChildNode(Node* node);
    void removeFromParent();
    void removeChildNode(Node* node);
    simd_float4x4 worldTransform() const;
    
};
