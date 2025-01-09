//
//  window.cpp
//  Drawing2D
//
//  Created by Taanish Reja on 1/8/25.
//

#include "window.h"

Renderer::Renderer(MTL::Device* device, MTK::View* view):
    m_device{device->retain()},
    m_view{view}

{
    m_commandQueue = m_device->newCommandQueue();
    
    makePipeline();
    makeResources();
}

Renderer::~Renderer()
{
    m_commandQueue->release();
    m_device->release();
}

void Renderer::makePipeline()
{
    MTL::Library* library = m_device->newDefaultLibrary();
    MTL::RenderPipelineDescriptor* renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    
    MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
    vertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormat::VertexFormatFloat2);
    vertexDescriptor->attributes()->object(0)->setOffset(0);
    vertexDescriptor->attributes()->object(0)->setBufferIndex(0);
    
    vertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormat::VertexFormatFloat4);
    vertexDescriptor->attributes()->object(1)->setOffset(sizeof(float)*2);
    vertexDescriptor->attributes()->object(1)->setBufferIndex(0);
    
    vertexDescriptor->layouts()->object(0)->setStride(sizeof(float)*6);

    renderPipelineDescriptor->setVertexDescriptor(vertexDescriptor);
    
    MTL::Function* vertexFunction = library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding));
    
    renderPipelineDescriptor->setVertexFunction(vertexFunction);
    
    MTL::Function* fragmentFunction = library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding));
    
    renderPipelineDescriptor->setFragmentFunction(fragmentFunction);
    
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(m_view->colorPixelFormat());
    
    NS::Error* error = nullptr;
    
    m_renderPipelineState = m_device->newRenderPipelineState(renderPipelineDescriptor, &error);
    
    if (error)
        std::cerr << "Couldn't make render pipeline state";
    
    library->release();
    renderPipelineDescriptor->release();
    vertexDescriptor->release();
    vertexFunction->release();
    fragmentFunction->release();
}

void Renderer::makeResources()
{
    std::vector<float> positions {
        //  x     y    r    g    b    a
            -0.8, 0.4, 1.0, 0.0, 1.0, 1.0,
            0.4, -0.8, 0.0, 1.0, 1.0, 1.0,
            0.8,  0.8, 1.0, 1.0, 0.0, 1.0,
    };
    
    
    m_vertexBuffer = m_device->newBuffer(positions.data(), sizeof(float) * positions.size(), MTL::StorageModeShared);
}


void Renderer::draw()
{
    // these objects are not retained, so must use autorelease pool (dont start with new or whatever)
    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();
    
    MTL::CommandBuffer* commandBuffer = m_commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPassDescriptor = m_view->currentRenderPassDescriptor();
    // render pass descriptor (commands we havent used that do not involve copying memory and such ig)
    
    MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    encoder->setRenderPipelineState(m_renderPipelineState);
    encoder->setVertexBuffer(m_vertexBuffer, 0, 0);
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::Integer(0), NS::Integer(3));
    encoder->endEncoding();
    // presentDrawable (show the drawable stuff as early as possible?) Looks like drawables are textures, while render descriptors are just previous stuff (lines, points, etc...?)
    commandBuffer->presentDrawable(m_view->currentDrawable());
    commandBuffer->commit();
    
    autoreleasePool->release();
}

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

//private:
//    NS::Window* window;
//    MTK::View* mtkView;
//    MTL::Device* device;
//    MTKViewDelegate* viewDelegate;
//};

