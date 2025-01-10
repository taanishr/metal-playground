//
//  MDLVertexDescriptor.h
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/9/25.
//


#ifndef MDLVERTEXDESCRIPTOR_H
#define MDLVERTEXDESCRIPTOR_H
#include "ModelIO.h"
#include <memory>
#include <iostream>

namespace ModelIO {
    enum class MDLGeometryType {
        points,
        lines,
        triangles,
        triangleStrips,
        quads,
        variableToplogy
    };

    enum class MDLVertexFormat {
        float2=786434,
        float3,
        float4
    };

    class MDLVertexLayout {
    public:
        MDLVertexLayout(int stride);
        
        int getStride() const;
    private:
        int m_stride;
    };

    class MDLVertexAttribute {
    public:
        MDLVertexAttribute(std::string name, MDLVertexFormat vertexFormat, int offset, int bufferIndex);
        ~MDLVertexAttribute();
        
        const std::string& getName() const;
        MDLVertexFormat getVertexFormat() const;
        int getOffset() const;
        int getBufferIndex() const;
        SwiftObject* getVertexAttribute();
    private:
        std::string m_name;
        MDLVertexFormat m_vertexFormat;
        int m_offset;
        int m_bufferIndex;
        std::shared_ptr<SwiftObject> m_vertexAttribute;
    };

    class MDLVertexDescriptor {
    public:
        MDLVertexDescriptor();
        ~MDLVertexDescriptor();
        
        void setAttribute(int index, MDLVertexAttribute& vertexAttribute);
        const MDLVertexAttribute& getAttribute(int index) const;
        void setLayout(int index, MDLVertexLayout& vertexLayout);
        const MDLVertexLayout& getLayout(int index) const;
        SwiftObject* getVertexDescriptor();
    //        MTL::VertexDescriptor* createMTLVertexDescriptor(); // must release
    private:
        std::shared_ptr<SwiftObject> m_vertexDescriptor;
        std::vector<MDLVertexAttribute> m_attributes;
        std::vector<MDLVertexLayout> m_layouts;
    };
}

#endif
