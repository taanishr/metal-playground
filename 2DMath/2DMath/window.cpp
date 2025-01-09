//
//  window.cpp
//  Drawing2D
//
//  Created by Taanish Reja on 1/8/25.
//

#include "window.h"

MTKViewDelegate::MTKViewDelegate(MTL::Device* device, MTK::View* view):
    MTK::ViewDelegate{},
    m_view{view},
    m_renderer{new Renderer{device, view}}
{
}

MTKViewDelegate::~MTKViewDelegate()
{
    delete m_renderer;
}

void MTKViewDelegate::drawInMTKView(MTK::View* view)
{
    m_renderer->draw();
}

AppDelegate::~AppDelegate()
{
    m_window->release();
    m_mtkView->release();
    delete m_viewDelegate;
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

    m_mtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
    m_mtkView->setClearColor(MTL::ClearColor::Make(0,0,0, 1));

    m_viewDelegate = new MTKViewDelegate{m_device, m_mtkView};
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
