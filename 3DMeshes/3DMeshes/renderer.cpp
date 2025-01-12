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
    m_constantsSize{sizeof(simd_float4x4)},
    m_constantsStride(align(m_constantsSize, 256)),
    m_constantsBufferOffset{0},
    m_frameSemaphore{1},
    m_time{0}
{
    m_commandQueue = m_device->newCommandQueue();
    
    makeResources();
    makePipeline();
}

Renderer::~Renderer()
{
    m_commandQueue->release();
    m_device->release();
    m_renderPipelineState->release();
    m_constantsBuffer->release();
    m_vertexDescriptor->release();
}

void Renderer::makeResources()
{
    ModelIO::MTKMeshBufferAllocator allocator {m_device};
    ModelIO::MDLMesh mdlMesh {ModelIO::Sphere{{1,1,1},{24,24},false, ModelIO::MDLGeometryType::triangles}, allocator};
    
    ModelIO::MDLVertexDescriptor vertexDescriptor {};
    vertexDescriptor.setAttribute(0, {ModelIO::MDLVertexAttributePosition, ModelIO::MDLVertexFormat::float3, 0, 0});
    
    std::cout << ModelIO::MDLVertexAttributePosition << '\n';
    std::cout << ModelIO::MDLVertexAttributeNormal << '\n';
    vertexDescriptor.setAttribute(1, {ModelIO::MDLVertexAttributeNormal, ModelIO::MDLVertexFormat::float3, 12, 0});
    vertexDescriptor.setLayout(0, ModelIO::MDLVertexLayout{24});
    
    m_vertexDescriptor = ModelIO::MDLtoMTLVertexDescriptor(vertexDescriptor);
    
    mdlMesh.setVertexDescriptor(vertexDescriptor);
    
    m_mesh = ModelIO::MTKMesh{mdlMesh, m_device};
    
    m_constantsBuffer = m_device->newBuffer(m_constantsStride*maxOutstandingFrameCount, MTL::StorageModeShared);
}

void Renderer::makePipeline()
{
    MTL::Library* library = m_device->newDefaultLibrary();
    MTL::RenderPipelineDescriptor* renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();

    renderPipelineDescriptor->setVertexDescriptor(m_vertexDescriptor);
    
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
    vertexFunction->release();
    fragmentFunction->release();
}

void Renderer::updateConstants()
{
//    m_time += 1.0 / m_view->preferredFramesPerSecond();
//    float timeF = float(m_time);
//    float pulseRate = 1.5f;
//    
//    float scaleFactor = 0.5f + 0.5f * cos(pulseRate*timeF);
//    simd_float2 scale = simd_float2{scaleFactor, scaleFactor};
//    simd_float4x4 scaleMatrix = simdHelpers::scale2D(scale);
//    
//    float rotationRate = 2.5f;
//    float rotationAngle = rotationRate * timeF;
//    simd_float4x4 rotationMatrix = simdHelpers::rotateZ(rotationAngle);
//    
//    float orbitalRadius = 0.0f;
//    simd_float2 translation = orbitalRadius * simd_float2{cos(timeF), sin(timeF)};
//    simd_float4x4 translationMatrix = simdHelpers::translate2D(translation);
//    
//    simd_float4x4 modelMatrix = matrix_multiply(translationMatrix,rotationMatrix);
    simd_float4x4 modelMatrix = matrix_identity_float4x4;
    
    float aspectRatio = float(m_view->drawableSize().width/m_view->drawableSize().height);
    float canvasWidth = 5.0f;
    float canvasHeight = canvasWidth/aspectRatio;
    simd_float4x4 projectionMatrix = simdHelpers::orthogonicProjection(-canvasWidth/2, canvasHeight/2, canvasWidth/2, -canvasHeight/2, -1.0, 1.0);
    
    simd_float4x4 transformationMatrix = matrix_multiply(projectionMatrix, modelMatrix);
    
    m_constantsBufferOffset = (m_frameIndex % maxOutstandingFrameCount) * m_constantsStride;
    uint8_t* rawConstantsPtr = static_cast<uint8_t*>(m_constantsBuffer->contents());
    simd_float4x4* constantsPtr = reinterpret_cast<simd_float4x4*>(rawConstantsPtr + m_constantsBufferOffset);
    std::memcpy(constantsPtr, &transformationMatrix, m_constantsSize);
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
    
    encoder->setVertexBuffer(m_constantsBuffer, m_constantsBufferOffset, 2);
    
    auto vertexBuffers = m_mesh.getVertexBuffers();
    auto submeshes = m_mesh.getSubmeshes();
    
    for (int i = 0; i < vertexBuffers.size(); ++i) {
        auto& vertexBuffer = vertexBuffers[i];
        encoder->setVertexBuffer(vertexBuffer.getBuffer(),vertexBuffers[i].getOffset(),i);
    }
    
    for (int i = 0; i < submeshes.size(); ++i) {
        auto& submesh = submeshes[i];
        auto& indexBuffer = submesh.getIndexBuffer();
        encoder->drawIndexedPrimitives(submesh.getPrimitiveType(), submesh.getIndexCount(), submesh.getIndexType(), indexBuffer.getBuffer(), indexBuffer.getOffset());
    }
    
    encoder->endEncoding();
    // presentDrawable (show the drawable stuff as early as possible?) Looks like drawables are textures, while render descriptors are just previous stuff (lines, points, etc...?)
    
    std::function<void(MTL::CommandBuffer*)> completedHandler = [this](MTL::CommandBuffer* cmdBuffer){
        this->m_frameSemaphore.release();
    };
    
    commandBuffer->presentDrawable(m_view->currentDrawable());
    commandBuffer->addCompletedHandler(completedHandler);
    commandBuffer->commit();
    
    ++m_frameIndex;
    
    autoreleasePool->release();
    
}
