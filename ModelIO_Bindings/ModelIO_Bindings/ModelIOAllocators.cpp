//
//  ModelIOAllocators.cpp
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/9/25.
//

#include "ModelIOAllocators.h"

namespace ModelIO {
    MTKMeshBufferAllocator::MTKMeshBufferAllocator(MTL::Device* device):
        m_allocator{std::make_shared<SwiftObject>(
            ModelIO_Bindings::createMTKMeshBufferAllocator(static_cast<void*>(device)),
            ModelIO_Bindings::releaseMTKMeshBufferAllocator
        )}
    {}

    MTKMeshBufferAllocator::~MTKMeshBufferAllocator() = default;

    SwiftObject* MTKMeshBufferAllocator::getAllocator() {
        return m_allocator.get();
    }
}

