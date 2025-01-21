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
m_constantsSize{sizeof(NodeConstants)},
    m_constantsStride{align(m_constantsSize, 256)},
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
    m_depthStencilState->release();
    m_vertexDescriptor->release();
    m_constantsBuffer->release();
    m_renderPipelineState->release();
    m_commandQueue->release();
    m_device->release();
}

void Renderer::makeResources()
{
    ModelIO::MTKMeshBufferAllocator allocator {m_device};
    
    ModelIO::MDLVertexDescriptor vertexDescriptor {};
    vertexDescriptor.setAttribute(0, {ModelIO::MDLVertexAttributePosition, ModelIO::MDLVertexFormat::float3, 0, 0});
    
    std::cout << ModelIO::MDLVertexAttributePosition << '\n';
    std::cout << ModelIO::MDLVertexAttributeNormal << '\n';
    vertexDescriptor.setAttribute(1, {ModelIO::MDLVertexAttributeNormal, ModelIO::MDLVertexFormat::float3, 12, 0});
    vertexDescriptor.setLayout(0, ModelIO::MDLVertexLayout{24});
    
    m_vertexDescriptor = ModelIO::MDLtoMTLVertexDescriptor(vertexDescriptor);
    
    ModelIO::MDLMesh sphereMdlMesh {ModelIO::Sphere{{1,1,1},{24,24},false, ModelIO::MDLGeometryType::triangles}, allocator};
    
    sphereMdlMesh.setVertexDescriptor(vertexDescriptor);
    
    ModelIO::MTKMesh sphereMTKMesh {sphereMdlMesh, m_device};
    
    sunNode.m_mesh = sphereMTKMesh;
    sunNode.m_color = simd_float4{1,1,0,1};
    
    planetNode.m_mesh = sphereMTKMesh;
    planetNode.m_color = simd_float4{0, 0.4, 0.9, 1};
    
    moonNode.m_mesh = sphereMTKMesh;
    moonNode.m_color = simd_float4{0.7,0.7,0.7,1};
    
    sunNode.addChildNode(&planetNode);
    planetNode.addChildNode(&moonNode);
    
    m_nodes.push_back(&sunNode);
    m_nodes.push_back(&planetNode);
    m_nodes.push_back(&moonNode);
    
    m_constantsBuffer = m_device->newBuffer(m_constantsStride*maxObjects*maxOutstandingFrameCount, MTL::StorageModeShared);
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
    
    renderPipelineDescriptor->setDepthAttachmentPixelFormat(m_view->depthStencilPixelFormat());
    
    NS::Error* error = nullptr;
    
    m_renderPipelineState = m_device->newRenderPipelineState(renderPipelineDescriptor, &error);
    
    if (error)
        std::cerr << "Couldn't make render pipeline state";
    
    MTL::DepthStencilDescriptor* depthStencilDescriptor = MTL::DepthStencilDescriptor::alloc()->init();
    depthStencilDescriptor->setDepthWriteEnabled(true);
    depthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
    m_depthStencilState = m_device->newDepthStencilState(depthStencilDescriptor);
    
    depthStencilDescriptor->release();
    fragmentFunction->release();
    vertexFunction->release();
    renderPipelineDescriptor->release();
    library->release();
}

int Renderer::constantBufferOffset(int objectIndex, int currentFrameIndex)
{
    int frameConstantsOffset = (currentFrameIndex % maxOutstandingFrameCount) * maxObjects * m_constantsStride;
    int objectConstantOffset = frameConstantsOffset + (objectIndex * m_constantsStride);
    return objectConstantOffset;
}

void Renderer::updateConstants()
{
    m_time += (1.0 / double(m_view->preferredFramesPerSecond()));
    float timeF = float(m_time);
    simd_float3 cameraPosition {0,0,7};
    simd_float4x4 viewMatrix = simdHelpers::translate(-cameraPosition);
    
    float aspectRatio = float(m_view->drawableSize().width/m_view->drawableSize().height);
    simd_float4x4 projectionMatrix = simdHelpers::perspectiveProjection(std::numbers::pi/3.0, aspectRatio, 0.01, 100);

    float planetRadius = 0.3;
    simd_float4x4 planetScale = simdHelpers::scale({planetRadius, planetRadius, planetRadius});
    
    float planetOrbitalRadius = 2;
    simd_float4x4 planetTranslate = simdHelpers::translate({planetOrbitalRadius, 0, 0});
    
    simd_float3 planetAxis {0,1,0};
    simd_float4x4 planetRotation = simdHelpers::rotateAbout(planetAxis, m_time);
    
    planetNode.m_transform = matrix_multiply(planetTranslate, planetRotation);
    planetNode.m_transform = matrix_multiply(planetNode.m_transform, planetScale);
    
    
    
//    simd_float3 rotationAxis = simd::normalize(simd::float3{0.3,0.7,0.2});
//    simd_float4x4 rotationMatrix = simdHelpers::rotateAbout(rotationAxis, timeF);
    
    
//    m_nodes[0].modelMatrix = matrix_multiply(simdHelpers::translate({-2,0,0}),rotationMatrix);
//    m_nodes[1].modelMatrix = matrix_multiply(simdHelpers::translate({2,0,0}), rotationMatrix);
//    
    for (int i = 0; i < m_nodes.size(); ++i) {
        simd_float4x4 transformationMatrix = matrix_multiply(projectionMatrix, viewMatrix);
    
        transformationMatrix = matrix_multiply(transformationMatrix, m_nodes[i]->worldTransform());
        
        NodeConstants constants {transformationMatrix, m_nodes[i]->m_color};
        
        int constantsBufferOffset = constantBufferOffset(i, m_frameIndex);
        
        uint8_t* rawConstantsPtr = static_cast<uint8_t*>(m_constantsBuffer->contents());
        simd_float4x4* constantsPtr = reinterpret_cast<simd_float4x4*>(rawConstantsPtr + constantsBufferOffset);
        
        std::memcpy(constantsPtr, &constants, m_constantsSize);
    }

    
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
    encoder->setDepthStencilState(m_depthStencilState);
    
    encoder->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);
    encoder->setCullMode(MTL::CullMode::CullModeBack);
    
    for (int i = 0; i < m_nodes.size(); ++i) {
        auto& mesh = m_nodes[i]->m_mesh;
        
        encoder->setVertexBuffer(m_constantsBuffer, constantBufferOffset(i, m_frameIndex), 2);
        
        auto vertexBuffers = mesh.getVertexBuffers();
        auto submeshes = mesh.getSubmeshes();
        
        for (int j = 0; j < vertexBuffers.size(); ++j) {
            auto& vertexBuffer = vertexBuffers[j];
            encoder->setVertexBuffer(vertexBuffer.getBuffer(),vertexBuffer.getOffset(),j);
        }
        
        for (int j = 0; j < submeshes.size(); ++j) {
            auto& submesh = submeshes[j];
            auto& indexBuffer = submesh.getIndexBuffer();
            encoder->drawIndexedPrimitives(submesh.getPrimitiveType(), submesh.getIndexCount(), submesh.getIndexType(), indexBuffer.getBuffer(), indexBuffer.getOffset());
        }
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
