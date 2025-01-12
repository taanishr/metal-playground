//
//  ModelIOMeshes.cpp
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/9/25.
//

#include "ModelIOMeshes.h"

namespace ModelIO {
    MDLMesh::MDLMesh(Sphere sphere, MTKMeshBufferAllocator& allocator):
        m_mesh{std::make_shared<SwiftObject>(
                ModelIO_Bindings::createSphereMDLMesh(
                sphere.sphereWithExtent.data(),
                sphere.segments.data(),
                sphere.inwardsNormal,
                static_cast<uint32_t>(sphere.geometryType),
                allocator.getAllocator()->get()
            ),
            ModelIO_Bindings::releaseMDLMesh
        )}
    {}

    MDLMesh::MDLMesh(Box box, MTKMeshBufferAllocator& allocator):
        m_mesh{std::make_shared<SwiftObject>(
                ModelIO_Bindings::createSphereMDLMesh(
                box.boxWithExtent.data(),
                box.segments.data(),
                box.inwardsNormal,
                static_cast<uint32_t>(box.geometryType),
                allocator.getAllocator()->get()
            ),
            ModelIO_Bindings::releaseMDLMesh
        )}
    {
        
    }
    
    MDLMesh::~MDLMesh() = default;

    SwiftObject* MDLMesh::retrieveMesh()
    {
        return m_mesh.get();
    }
    
    void MDLMesh::setVertexDescriptor(MDLVertexDescriptor& vertexDescriptor)
    {
        ModelIO_Bindings::setMDLMeshVertexDescriptor(m_mesh->get(), vertexDescriptor.getVertexDescriptor()->get());
        m_vertexDescriptor = vertexDescriptor;
    }

    MDLVertexDescriptor& MDLMesh::getVeretexDescriptor()
    {
        return m_vertexDescriptor;
    }

    MTKMeshBuffer::MTKMeshBuffer(MTL::Buffer* buffer, long offset):
        m_buffer{NS::RetainPtr(buffer)}, m_offset{offset} // retain does not take ownership, transfer does
    {}

    MTL::Buffer* MTKMeshBuffer::getBuffer()
    {
        return m_buffer.get();
    }

    long MTKMeshBuffer::getOffset() {
        return m_offset;
    }

    MTKMeshBuffer::~MTKMeshBuffer() = default;

    MTKSubmesh::MTKSubmesh(MTL::PrimitiveType primitiveType,
                      MTL::IndexType indexType,
                      const MTKMeshBuffer& indexBuffer,
                      long indexCount):
        m_primitiveType(primitiveType),
        m_indexType{indexType},
        m_indexBuffer{indexBuffer},
        m_indexCount{indexCount}
    {}

    MTKSubmesh::~MTKSubmesh() = default;


    MTL::PrimitiveType MTKSubmesh::getPrimitiveType()
    {
        return m_primitiveType;
    }

    MTL::IndexType MTKSubmesh::getIndexType()
    {
        return m_indexType;
    }

    MTKMeshBuffer& MTKSubmesh::getIndexBuffer()
    {
        return m_indexBuffer;
    }

    long MTKSubmesh::getIndexCount()
    {
        return m_indexCount;
    }

    MTKMesh::MTKMesh(MDLMesh& mesh, MTL::Device* device):
    m_mesh{std::make_shared<SwiftObject>(
        ModelIO_Bindings::createMTKMesh(mesh.retrieveMesh()->get(), static_cast<void*>(device)),
        ModelIO_Bindings::releaseMTKMesh
    )}
    {
        m_vertexCount = ModelIO_Bindings::getMTKMeshVertexCount(m_mesh->get());
        constructVertexBuffers();
        constructSubmeshes();
    }

    void MTKMesh::constructVertexBuffers()
    {
        long vertexBufferCount = ModelIO_Bindings::countVertexBuffers(m_mesh->get());
        void* vertexBuffers[vertexBufferCount];
        long offsets[vertexBufferCount];
        ModelIO_Bindings::extractMeshBuffers(m_mesh->get(), vertexBuffers, offsets);
        for (int i = 0; i < vertexBufferCount; ++i) {
            m_vertexBuffers.push_back({static_cast<MTL::Buffer*>(vertexBuffers[i]), offsets[i]});
        }
    }

    // TODO
    void MTKMesh::constructSubmeshes()
    {
        long submeshCount = ModelIO_Bindings::countSubmeshes(m_mesh->get());
        
        unsigned long primitiveTypes[submeshCount];
        unsigned long indexTypes[submeshCount];
        void* submeshBuffers[submeshCount];
        long submeshBufferOffsets[submeshCount];
        long indexCounts[submeshCount];
        
        ModelIO_Bindings::extractSubmeshPrimitiveTypes(m_mesh->get(), primitiveTypes);
        ModelIO_Bindings::extractSubmeshIndexTypes(m_mesh->get(), indexTypes);
        ModelIO_Bindings::extractSubmeshIndexBuffers(m_mesh->get(), submeshBuffers, submeshBufferOffsets);
        ModelIO_Bindings::extractSubmeshIndexCounts(m_mesh->get(), indexCounts);
        
        for (int i = 0; i < submeshCount; ++i) {
            m_submeshes.push_back(MTKSubmesh{
                                    static_cast<MTL::PrimitiveType>(primitiveTypes[i]),
                                    static_cast<MTL::IndexType>(indexTypes[i]),
                                    {static_cast<MTL::Buffer*>(submeshBuffers[i]), submeshBufferOffsets[i]},
                                    indexCounts[i]});
        }
        
    }
        
    MTKMesh::~MTKMesh() = default;
        
    std::vector<MTKMeshBuffer>& MTKMesh::getVertexBuffers()
    {
        return m_vertexBuffers;
    }
        
    std::vector<MTKSubmesh>& MTKMesh::getSubmeshes()
    {
        return m_submeshes;
    }
    
    SwiftObject* MTKMesh::retrieveMesh()
    {
        return m_mesh.get();
    }
}
