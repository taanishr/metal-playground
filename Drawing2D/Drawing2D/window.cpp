//
//  window.cpp
//  Drawing2D
//
//  Created by Taanish Reja on 1/8/25.
//

#include "window.h"

Renderer::Renderer(MTL::Device* device):
    m_device{device->retain()}
{
    m_commandQueue = m_device->newCommandQueue();
}

Renderer::~Renderer()
{
    m_commandQueue->release();
    m_device->release();
}

void Renderer::draw(MTK::View* view)
{
    // these objects are not retained, so must use autorelease pool (dont start with new or whatever)
    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();
    
    MTL::CommandBuffer* commandBuffer = m_commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPassDescriptor = view->currentRenderPassDescriptor();
    // render pass descriptor (commands we havent used that do not involve copying memory and such ig)
    MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    encoder->endEncoding();
    // presentDrawable (show the drawable stuff as early as possible?) Looks like drawables are textures, while render descriptors are just previous stuff (lines, points, etc...?)
    commandBuffer->presentDrawable(view->currentDrawable());
    commandBuffer->commit();
    
    autoreleasePool->release();
}

MTKViewDelegate::MTKViewDelegate(MTL::Device* device):
    MTK::ViewDelegate{},
    m_renderer{new Renderer{device}}
{
    
}

MTKViewDelegate::~MTKViewDelegate()
{
    delete m_renderer;
}

void MTKViewDelegate::drawInMTKView(MTK::View* view)
{
    m_renderer->draw(view);
}

AppDelegate::~AppDelegate()
{
    m_window->release();
//    m_mtkView->release();
//    delete m_viewDelegate;
}

void AppDelegate::applicationWillFinishLaunching(NS::Notification* notification)
{
    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->setActivationPolicy(NS::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(NS::Notification* notification)
{
    CGRect frame = CGRect{{100.0, 100.0}, {512.0, 512.0}};
    
    m_window = NS::Window::alloc()->init(
                frame,
                NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled,
                NS::BackingStoreBuffered,
                false
             );
    
    m_device = MTL::CreateSystemDefaultDevice();
    
    m_mtkView = MTK::View::alloc()->init(frame, m_device);

    m_mtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    m_mtkView->setClearColor(MTL::ClearColor::Make(0, 0.5, 1.0, 1.0));

    m_viewDelegate = new MTKViewDelegate{m_device};
    m_mtkView->setDelegate(m_viewDelegate);
    
    m_window->setContentView(m_mtkView);
    
    m_window->setTitle(NS::String::string("This shit so adds, part 2", NS::StringEncoding::UTF8StringEncoding ));
//    
    m_window->makeKeyAndOrderFront(nullptr);
    
    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    
    app->activateIgnoringOtherApps(true);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender)
{
    return true;
}

//private:
//    NS::Window* window;
//    MTK::View* mtkView;
//    MTL::Device* device;
//    MTKViewDelegate* viewDelegate;
//};

