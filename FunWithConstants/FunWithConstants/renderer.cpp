//
//  renderer.cpp
//  FunWithConstants
//
//  Created by Taanish Reja on 1/8/25.
//

#include "renderer.h"

Renderer::Renderer(MTL::Device* device, MTK::View* view):
    m_device{device->retain()},
    m_view{view},
    m_frameIndex{0},
    m_constantsSize{sizeof(simd_float2)},
    m_constantsStride(align(m_constantsSize, 256)),
    m_constantsBufferOffset{0},
    m_frameSemaphore{1}
{
    m_commandQueue = m_device->newCommandQueue();
    
    makePipeline();
    makeResources();
}

Renderer::~Renderer()
{
    m_commandQueue->release();
    m_device->release();
    m_renderPipelineState->release();
    m_vertexBuffer->release();
    m_constantsBuffer->release();
}

void Renderer::makePipeline()
{
    MTL::Library* library = m_device->newDefaultLibrary();
    MTL::RenderPipelineDescriptor* renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    
    MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
    vertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormat::VertexFormatFloat2);
    vertexDescriptor->attributes()->object(0)->setOffset(0);
    vertexDescriptor->attributes()->object(0)->setBufferIndex(0);
    
    vertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormat::VertexFormatFloat4);
    vertexDescriptor->attributes()->object(1)->setOffset(sizeof(float)*2);
    vertexDescriptor->attributes()->object(1)->setBufferIndex(0);
    
    vertexDescriptor->layouts()->object(0)->setStride(sizeof(float)*6);

    renderPipelineDescriptor->setVertexDescriptor(vertexDescriptor);
    
    MTL::Function* vertexFunction = library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding));
    
    renderPipelineDescriptor->setVertexFunction(vertexFunction);
    
    MTL::Function* fragmentFunction = library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding));
    
    renderPipelineDescriptor->setFragmentFunction(fragmentFunction);
    
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(m_view->colorPixelFormat());
    
    NS::Error* error = nullptr;
    
    m_renderPipelineState = m_device->newRenderPipelineState(renderPipelineDescriptor, &error);
    
    if (error)
        std::cerr << "Couldn't make render pipeline state";
    
    library->release();
    renderPipelineDescriptor->release();
    vertexDescriptor->release();
    vertexFunction->release();
    fragmentFunction->release();
}

void Renderer::makeResources()
{
    std::vector<float> positions {
        //  x     y    r    g    b    a
            -0.8, 0.4, 1.0, 0.0, 1.0, 1.0,
            0.4, -0.8, 0.0, 1.0, 1.0, 1.0,
            0.8,  0.8, 1.0, 1.0, 0.0, 1.0,
    };
    
    
    m_vertexBuffer = m_device->newBuffer(positions.data(), sizeof(float) * positions.size(), MTL::StorageModeShared);
    
    m_constantsBuffer = m_device->newBuffer(m_constantsStride*maxOutstandingFrameCount, MTL::StorageModeShared);
}

void Renderer::updateConstants()
{
    auto time = std::chrono::high_resolution_clock::now();
    float timeF = std::chrono::duration<float>(time.time_since_epoch()).count();
    float speedFactor = 2.0f;
    float rotationAngle = float(fmod(speedFactor*timeF, std::numbers::pi*2.0f));
    float rotationMagnitude = .5f;
    simd_float2 positionOffset = rotationMagnitude*simd_float2{cos(rotationAngle), sin(rotationAngle)};
    m_constantsBufferOffset = (m_frameIndex % maxOutstandingFrameCount) * m_constantsStride;
    uint8_t* rawConstantsPtr = static_cast<uint8_t*>(m_constantsBuffer->contents());
    simd_float2* constantsPtr = reinterpret_cast<simd_float2*>(rawConstantsPtr + m_constantsBufferOffset);
    std::memcpy(constantsPtr, &positionOffset, m_constantsSize);
}


void Renderer::draw()
{
    // these objects are not retained, so must use autorelease pool (dont start with new or whatever)
    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();
    
    m_frameSemaphore.acquire();
    
    updateConstants();
    
    MTL::CommandBuffer* commandBuffer = m_commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPassDescriptor = m_view->currentRenderPassDescriptor();
    // render pass descriptor (commands we havent used that do not involve copying memory and such ig)
    
    MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    encoder->setRenderPipelineState(m_renderPipelineState);
    encoder->setVertexBuffer(m_vertexBuffer, 0, 0);
    encoder->setVertexBuffer(m_constantsBuffer, m_constantsBufferOffset, 1);
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::Integer(0), NS::Integer(3));
    encoder->endEncoding();
    // presentDrawable (show the drawable stuff as early as possible?) Looks like drawables are textures, while render descriptors are just previous stuff (lines, points, etc...?)
    
    std::function<void(MTL::CommandBuffer*)> completedHandler = [this](MTL::CommandBuffer* cmdBuffer){
        this->m_frameSemaphore.release();
    };
    
    commandBuffer->addCompletedHandler(completedHandler);
    commandBuffer->presentDrawable(m_view->currentDrawable());
    commandBuffer->commit();
    
    ++m_frameIndex;
    
    autoreleasePool->release();
    
}
