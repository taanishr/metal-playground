//
//  window.h
//  Drawing2D
//
//  Created by Taanish Reja on 1/8/25.
//

#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <functional>
#include "metal_imports.h"
#include "renderer.h"


class MTKViewDelegate : public MTK::ViewDelegate {
public:
    MTKViewDelegate(MTL::Device* device, MTK::View* view);
    ~MTKViewDelegate() override;
    void drawInMTKView(MTK::View* view) override;
private:
    Renderer* m_renderer;
    MTK::View* m_view;
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

