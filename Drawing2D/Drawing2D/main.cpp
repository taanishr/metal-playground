//
//  main.cpp
//  Drawing2D
//
//  Created by Taanish Reja on 1/8/25.
//

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "window.h"

int main(int argc, const char * argv[]) {
    sleep(1);
    
    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();
    
    AppDelegate appDelegate;

    NS::Application* app = NS::Application::sharedApplication();
    
    app->setDelegate(&appDelegate);
    
    app->run();
    
    autoreleasePool->release();
}
