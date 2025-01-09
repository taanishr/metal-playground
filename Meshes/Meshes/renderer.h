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
#include "mesh.h"

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
    MTL::Device* m_device;
    MTL::CommandQueue* m_commandQueue;
    MTL::RenderPipelineState* m_renderPipelineState;
    SimpleMesh m_mesh;
    MTK::View* m_view;
    int m_frameIndex;
    MTL::Buffer* m_constantsBuffer;
    int m_constantsSize;
    int m_constantsStride;
    int m_constantsBufferOffset;
    std::counting_semaphore<maxOutstandingFrameCount> m_frameSemaphore;
    double m_time;
};

#endif
