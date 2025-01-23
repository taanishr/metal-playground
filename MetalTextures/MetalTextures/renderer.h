//
//  renderer.h
//  FunWithConstants
//
//  Created by Taanish Reja on 1/8/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include "metal_imports.h"
#include <semaphore>
#include <iostream>
#include "utilities.h"
#include <chrono>
#include <numbers>
#include <ModelIO.h>
#include "node.h"
#include <MTKTextureLoader.h>

constexpr int maxObjects = 16;
constexpr int maxOutstandingFrameCount = 3;

class Renderer {
public:
    Renderer(MTL::Device* device, MTK::View* view);
    ~Renderer();
    void draw();
    void updateConstants();
    void makePipeline();
    void makeResources();
private:
    int constantBufferOffset(int objectIndex, int currentFrameIndex);
    
    MTL::Device* m_device;
    MTL::CommandQueue* m_commandQueue;
    MTL::RenderPipelineState* m_renderPipelineState;
//    ModelIO::MTKMesh m_mesh;
    std::vector<Node> m_nodes;
    MTK::View* m_view;
    int m_frameIndex;
    MTL::Buffer* m_constantsBuffer;
    int m_constantsSize;
    int m_constantsStride;
    int m_constantsBufferOffset;
    MTL::VertexDescriptor* m_vertexDescriptor;
    std::counting_semaphore<maxOutstandingFrameCount> m_frameSemaphore;
    double m_time;
    MTL::DepthStencilState* m_depthStencilState;
    MTL::SamplerState* m_samplerState;
};

#endif
