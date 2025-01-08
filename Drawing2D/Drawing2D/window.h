//
//  window.h
//  Drawing2D
//
//  Created by Taanish Reja on 1/8/25.
//

#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include "metal_imports.h"
class Renderer {
public:
    Renderer(MTL::Device* device);
    ~Renderer();
    void draw(MTK::View* view);
private:
    MTL::Device* m_device;
    MTL::CommandQueue* m_commandQueue;
};

class MTKViewDelegate : public MTK::ViewDelegate {
public:
    MTKViewDelegate(MTL::Device* device);
    ~MTKViewDelegate() override;
    void drawInMTKView(MTK::View* view) override;
private:
    Renderer* m_renderer;
};

class AppDelegate : public NS::ApplicationDelegate {
public:
    ~AppDelegate();
    
    void applicationWillFinishLaunching(NS::Notification* notification) override;
    void applicationDidFinishLaunching(NS::Notification* notification) override;
    bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;
private:
    NS::Window* m_window;
    MTK::View* m_mtkView;
    MTL::Device* m_device;
    MTKViewDelegate* m_viewDelegate;
};

#endif

