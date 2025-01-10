//
//  MDLVertexDescriptor.swift
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/8/25.
//

import MetalKit
import ModelIO

// MDL Vertex Attribute
// create
public func createMDLVertexAttribute(namePtr: UnsafePointer<CChar>, rawFormat: UInt, offset: Int, bufferIndex: Int) -> UnsafeMutableRawPointer
{
    let name = String(cString: namePtr);
    let format = MDLVertexFormat(rawValue: rawFormat)!;
    let mdlVertexAttribute = MDLVertexAttribute(name: name, format: format, offset: offset, bufferIndex: bufferIndex);
    let mdlVertexAttributePtr = UnsafeMutableRawPointer(Unmanaged.passRetained(mdlVertexAttribute).toOpaque());
    return mdlVertexAttributePtr;
}

// release
public func releaseMDLVertexAttribute(mdlVertexAttributePtr: UnsafeMutableRawPointer)
{
    let mdlVertexAttribute = Unmanaged<MDLVertexAttribute>.fromOpaque(mdlVertexAttributePtr);
    mdlVertexAttribute.release();
}

// MDL Vertex Descriptor
// create
public func createMDLVertexDescriptor() -> UnsafeMutableRawPointer
{
    let mdlVertexDescriptor = MDLVertexDescriptor();
    let mdlVertexDescriptorPtr = Unmanaged<MDLVertexDescriptor>.passRetained(mdlVertexDescriptor).toOpaque();
    return mdlVertexDescriptorPtr;
}

// MDL Vertex Layout
// set layout
public func setMDLVertexLayout(mdlVertexDescriptorPtr: UnsafeMutableRawPointer, index: Int, stride: Int)
{
    let mdlVertexDescriptor = Unmanaged<MDLVertexDescriptor>.fromOpaque(mdlVertexDescriptorPtr).takeUnretainedValue();
    let newLayout = MDLVertexBufferLayout(stride: stride);
    mdlVertexDescriptor.layouts[index] = newLayout;
}

// set vertex attribute
public func setMDLVertexDescriptorAttribute(mdlVertexDescriptorPtr: UnsafeMutableRawPointer, index: Int, mdlVertexAttributePtr: UnsafeMutableRawPointer)
{
    let mdlVertexDescriptor = Unmanaged<MDLVertexDescriptor>.fromOpaque(mdlVertexDescriptorPtr).takeUnretainedValue();
    let mdlVertexAttribute = Unmanaged<MDLVertexAttribute>.fromOpaque(mdlVertexAttributePtr).takeUnretainedValue(); // must release attribute seperately
    mdlVertexDescriptor.attributes[index] = mdlVertexAttribute;
}

// release
public func releaseMDLVertexDescriptor(mdlVertexDescriptor: UnsafeMutableRawPointer)
{
    let mdlVertexDescriptorPtr = Unmanaged<MDLVertexDescriptor>.fromOpaque(mdlVertexDescriptor);
    mdlVertexDescriptorPtr.release();
}
