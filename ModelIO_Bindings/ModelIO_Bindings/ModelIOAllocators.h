//
//  ModelIOAllocators.h
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/9/25.
//


#ifndef MODELIOALLOCATORS_H
#define MODELIOALLOCATORS_H
#include "ModelIO.h"

namespace ModelIO {
    class MTKMeshBufferAllocator {
    public:
        MTKMeshBufferAllocator(MTL::Device* device);
        ~MTKMeshBufferAllocator();
        
        SwiftObject* getAllocator();
    private:
        std::shared_ptr<SwiftObject> m_allocator;
    };
}

#endif
