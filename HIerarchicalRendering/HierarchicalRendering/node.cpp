//
//  node.cpp
//  HIerarchicalRendering
//
//  Created by Taanish Reja on 1/16/25.
//

#include "node.h"

NodeConstants::NodeConstants(simd_float4x4 modelViewProjectionMatrix, simd_float4 color):
    m_modelViewProjectionMatrix{modelViewProjectionMatrix},
    m_color{color}
{};

Node::Node(ModelIO::MTKMesh mesh):
    m_mesh{mesh}, m_parentNode{nullptr}
{}

void Node::addChildNode(Node* node)
{
    m_nodes.push_back(node);
    node->m_parentNode = this;
}

void Node::removeFromParent()
{
    if (m_parentNode) {
        m_parentNode->removeChildNode(this);
    }
}

void Node::removeChildNode(Node* node)
{
    auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
    if (it != m_nodes.end()) {
        (*it)->m_parentNode = nullptr;
        m_nodes.erase(it);
    }
}

simd_float4x4 Node::worldTransform() const
{
    if (m_parentNode) {
        return matrix_multiply(m_transform, m_parentNode->m_transform);
    }else {
        return m_transform;
    }
}


