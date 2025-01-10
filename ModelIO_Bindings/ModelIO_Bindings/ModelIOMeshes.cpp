//
//  ModelIOMeshes.cpp
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/9/25.
//

#include "ModelIOMeshes.h"

namespace ModelIO {
    MDLMesh(Sphere sphere, const MDLMeshBufferAllocator& allocator):
        m_mesh
    {}

    MDLMesh(Box box, const MDLMeshBufferAllocator& allocator);
    
    SwiftObject& retrieveMesh();
    
    void setVertexDescriptor(const MDLVertexDescriptor&);
    
    ~MDLMesh();
}
