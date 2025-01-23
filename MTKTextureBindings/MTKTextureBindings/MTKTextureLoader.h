//
//  MTKTextureLoader.h
//  MTKTextureBindings
//
//  Created by Taanish Reja on 1/21/25.
//

#include <SwiftObject.h>
#include <metal_imports.h>

namespace MTKTextures {
    class MTKTextureLoader {
    public:
        MTKTextureLoader() = default;
        MTKTextureLoader(MTL::Device*);
        ~MTKTextureLoader();
        
        SwiftObject* getTextureLoader() const;
    private:
        std::shared_ptr<SwiftObject> m_textureLoader;
    };

    MTL::Texture* createTexture(const MTKTextureLoader& textureLoader, 
                                const std::string& filepath);
}
