//
//  ModelIOMeshes.swift
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/8/25.
//

import ModelIO
import MetalKit
import Metal

// MDL Mesh

// create
// sphere with extent
// (sphereWithExtent: <#T##vector_float3#>, segments: <#T##vector_uint2#>, inwardNormals: <#T##Bool#>, geometryType: <#T##MDLGeometryType#>, allocator: <#T##MDLMeshBufferAllocator?#>)

public func createSphereMDLMesh(
    sphereWithExtent: simd_float3,
    segments: simd_uint2,
    inwardNormals: Bool,
    rawGeometryType: Int,
    allocatorPtr: UnsafeMutableRawPointer
    
) -> UnsafeMutableRawPointer
{
    let allocator = Unmanaged<MTKMeshBufferAllocator>.fromOpaque(allocatorPtr).takeUnretainedValue();
    let geometryType = MDLGeometryType(rawValue: rawGeometryType)!;
    let mdlMesh = MDLMesh(sphereWithExtent: sphereWithExtent, segments: segments, inwardNormals: inwardNormals, geometryType: geometryType, allocator: allocator);
    let mdlMeshPtr = UnsafeMutableRawPointer(Unmanaged.passRetained(mdlMesh).toOpaque());
    return mdlMeshPtr;
}


// box with extent
//(boxWithExtent: <#T##vector_float3#>, segments: <#T##vector_uint3#>, inwardNormals: <#T##Bool#>, geometryType: <#T##MDLGeometryType#>, allocator: MDLMeshBufferAllocator?)

public func createBoxMDLMesh(
    boxWithExtent: simd_float3,
    segments: simd_uint3,
    inwardNormals: Bool,
    rawGeometryType: Int,
    allocatorPtr: UnsafeMutableRawPointer
) -> UnsafeMutableRawPointer
{
    let allocator = Unmanaged<MTKMeshBufferAllocator>.fromOpaque(allocatorPtr).takeUnretainedValue();
    let geometryType = MDLGeometryType(rawValue: rawGeometryType)!;
    let mdlMesh = MDLMesh(boxWithExtent: boxWithExtent, segments: segments, inwardNormals: inwardNormals, geometryType: geometryType, allocator: allocator);
    let mdlMeshPtr = UnsafeMutableRawPointer(Unmanaged.passRetained(mdlMesh).toOpaque());
    return mdlMeshPtr;
}

// release
public func releaseMDLMesh(mdlMeshPtr: UnsafeMutableRawPointer)
{
    let mdlMesh = Unmanaged<MDLMesh>.fromOpaque(mdlMeshPtr);
    mdlMesh.release();
}

// MTKMeshBuffer
// MTLVertexBuffer*
// int offset

// MTKMesh
// create
// (MDLMesh, device)
public func createMTKMesh(mdlMeshPtr: UnsafeMutableRawPointer, devicePtr: UnsafeMutableRawPointer) -> UnsafeMutableRawPointer?
{
    let device = Unmanaged<MTLDevice>.fromOpaque(mdlMeshPtr).takeUnretainedValue();
    let mdlMesh = Unmanaged<MDLMesh>.fromOpaque(mdlMeshPtr).takeUnretainedValue();
    
    guard let mtkMesh = try? MTKMesh(mesh: mdlMesh, device: device)
    else {
        return nil;
    };
    
    let mtkMeshPtr = UnsafeMutableRawPointer(Unmanaged.passRetained(mtkMesh).toOpaque());
    
    return mtkMeshPtr;
}

// getVertexBuffers: [MTKMeshBuffer]
// getVertexDescriptor: MDLVertexDescriptor
// var submeshes: [MTKSubmesh]
// var vertexCount: Int
// c++ equivalents created on construction by calling private helpers

// release

// will be made in c++, doesnt need swift equivalent, but need to be able to get erquired data
// MTKSubmesh
//var primitiveType: MTLPrimitiveType
//var indexType: MTLIndexType
//var indexBuffer: MTKMeshBuffer
//var indexCount: Int





