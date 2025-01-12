//
//  ModelIOMeshes.h
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/9/25.
//

#ifndef MODELIOMESHES_H
#define MODELIOMESHES_H
#include "ModelIO_Internal.h"
#include "MDLVertexDescriptor.h"
#include "ModelIOAllocators.h"

namespace ModelIO {
    struct Sphere {
        std::array<float, 3> sphereWithExtent;
        std::array<uint32_t, 2>  segments;
        bool inwardsNormal;
        MDLGeometryType geometryType;
    };

    struct Box {
        std::array<float, 3> boxWithExtent;
        std::array<uint32_t, 3> segments;
        bool inwardsNormal;
        MDLGeometryType geometryType;
    };

    class MDLMesh {
    public:
        MDLMesh(Sphere sphere, MTKMeshBufferAllocator& allocator);
        
        MDLMesh(Box box, MTKMeshBufferAllocator& allocator);
        
        ~MDLMesh();
        
        SwiftObject* retrieveMesh();
        
        void setVertexDescriptor(MDLVertexDescriptor& vertexDescriptor);
        
        MDLVertexDescriptor& getVeretexDescriptor();
    private:
        MDLVertexDescriptor m_vertexDescriptor;
        std::shared_ptr<SwiftObject> m_mesh;
    };

    class MTKMeshBuffer {
    public:
        MTKMeshBuffer(MTL::Buffer* buffer, long offset);
        ~MTKMeshBuffer();
        
        MTL::Buffer* getBuffer();
        long getOffset();
    private:
        NS::SharedPtr<MTL::Buffer> m_buffer;
        long m_offset;
    };

    class MTKSubmesh {
    public:
        MTKSubmesh(MTL::PrimitiveType primitiveType,
                      MTL::IndexType indexType,
                      const MTKMeshBuffer& indexBuffer,
                      long indexCount);
        ~MTKSubmesh();
        
        MTL::PrimitiveType getPrimitiveType();
        MTL::IndexType getIndexType();
        MTKMeshBuffer& getIndexBuffer();
        long getIndexCount();
    private:
        MTL::PrimitiveType m_primitiveType;
        MTL::IndexType m_indexType;
        MTKMeshBuffer m_indexBuffer;
        long m_indexCount;
    };

    class MTKMesh {
    public:
        MTKMesh() = default;
        
        MTKMesh(MDLMesh& mesh, MTL::Device* device);
        
        ~MTKMesh();
        
        std::vector<MTKMeshBuffer>& getVertexBuffers();
        
        std::vector<MTKSubmesh>& getSubmeshes();
        
        SwiftObject* retrieveMesh();
    private:
        void constructVertexBuffers();
        void constructSubmeshes();
        
        std::shared_ptr<SwiftObject> m_mesh;
        std::vector<MTKMeshBuffer> m_vertexBuffers;
        std::vector<MTKSubmesh> m_submeshes;
        long m_vertexCount;
    };
}
#endif
