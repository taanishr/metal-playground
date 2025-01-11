//
//  MDLVertexDescriptor.cpp
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/9/25.
//

#include "MDLVertexDescriptor.h"

namespace ModelIO {
    MDLVertexLayout::MDLVertexLayout(int stride):
        m_stride{0}
    {}
        
    int MDLVertexLayout::getStride() const
    {
        return m_stride;
    }

    MDLVertexAttribute::MDLVertexAttribute(std::string name, MDLVertexFormat vertexFormat, int offset, int bufferIndex):
        m_name{name},
        m_vertexFormat{vertexFormat},
        m_offset{offset},
        m_bufferIndex{bufferIndex},
        m_vertexAttribute{
            std::make_shared<SwiftObject>(
            ModelIO_Bindings::createMDLVertexAttribute(name.c_str(), static_cast<unsigned long>(vertexFormat), offset, bufferIndex),
            ModelIO_Bindings::releaseMDLVertexDescriptor
            )
        }
    {}

    MDLVertexAttribute::~MDLVertexAttribute() = default;
    
    const std::string& MDLVertexAttribute::getName() const
    {
        return m_name;
    }

    MDLVertexFormat MDLVertexAttribute::getVertexFormat() const
    {
        return m_vertexFormat;
    }

    int MDLVertexAttribute::getOffset() const
    {
        return m_offset;
    }

    int MDLVertexAttribute::getBufferIndex() const
    {
        return m_bufferIndex;
    }

    SwiftObject* MDLVertexAttribute::getVertexAttribute()
    {
        return m_vertexAttribute.get();
    }

    MDLVertexDescriptor::MDLVertexDescriptor():
        m_vertexDescriptor{
            std::make_shared<SwiftObject>(
                 ModelIO_Bindings::createMDLVertexDescriptor(),
                 ModelIO_Bindings::releaseMDLVertexDescriptor
            )
        }
    {}

    MDLVertexDescriptor::~MDLVertexDescriptor() = default;

    void MDLVertexDescriptor::setAttribute(int index, MDLVertexAttribute& vertexAttribute)
    {
        if (index >= m_attributes.size()) {
            m_attributes.push_back(vertexAttribute);
        }else {
            m_attributes[index] = vertexAttribute;
        }
        
        ModelIO_Bindings::setMDLVertexDescriptorAttribute(m_vertexDescriptor->get(), index, vertexAttribute.getVertexAttribute()->get());
    }

    const MDLVertexAttribute& MDLVertexDescriptor::getAttribute(int index) const
    {
        return m_attributes[index];
    }

    void MDLVertexDescriptor::setLayout(int index, MDLVertexLayout& vertexLayout)
{
        if (index >= m_layouts.size()) {
            m_layouts.push_back(vertexLayout);
        }else {
            m_layouts[index] = vertexLayout;
        }
        
        ModelIO_Bindings::setMDLVertexLayout(m_vertexDescriptor->get(), index, vertexLayout.getStride());
    }

    const MDLVertexLayout& MDLVertexDescriptor::getLayout(int index) const
    {
        return m_layouts[index];
    }

    SwiftObject* MDLVertexDescriptor::getVertexDescriptor()
    {
        return m_vertexDescriptor.get();
    }
}
