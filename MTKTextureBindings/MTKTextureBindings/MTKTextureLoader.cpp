//
//  MTKTextureLoader.cpp
//  MTKTextureBindings
//
//  Created by Taanish Reja on 1/21/25.
//

#include "MTKTextureLoader.h"
#include <MTKTextureBindings-Swift.h>
#include <iostream>

namespace MTKTextures {
    MTKTextureLoader::MTKTextureLoader(MTL::Device* device):
    m_textureLoader{std::make_shared<SwiftObject>
        (MTKTextureBindings::createMTKTextureLoader(static_cast<void*>(device)),
         MTKTextureBindings::releaseMTKTextureLoader)
    }
    {}

    MTKTextureLoader::~MTKTextureLoader() = default;

    SwiftObject* MTKTextureLoader::getTextureLoader() const
    {
        return m_textureLoader.get();
    }

    MTL::Texture* createTexture(const MTKTextureLoader& textureLoader,
                                const std::string& filepath)
    {
        
        MTL::Texture* texture = static_cast<MTL::Texture*>
            (MTKTextureBindings::createTexture(
                                        textureLoader.getTextureLoader()->get(),
                                        filepath.c_str()
            )
        );
        return texture;
    }
}
