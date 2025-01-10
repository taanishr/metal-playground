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

func pointerToSIMD<T: SIMD>(
    pointer: UnsafePointer<T.Scalar>
) -> T where T.Scalar: Numeric {
    let simdVector = T((0..<T.scalarCount).map { pointer[$0] });
    return simdVector;
}

public func createSphereMDLMesh(
    sphereWithExtentPtr: UnsafePointer<Float>,
    segmentsPtr: UnsafePointer<UInt32>,
    inwardNormals: Bool,
    rawGeometryType: Int,
    allocatorPtr: UnsafeMutableRawPointer
    
) -> UnsafeMutableRawPointer
{
    let allocator = Unmanaged<MTKMeshBufferAllocator>.fromOpaque(allocatorPtr).takeUnretainedValue();
    let geometryType = MDLGeometryType(rawValue: rawGeometryType)!;
    let mdlMesh = MDLMesh(sphereWithExtent: pointerToSIMD(pointer: sphereWithExtentPtr),
                          segments: pointerToSIMD(pointer: segmentsPtr),
                          inwardNormals: inwardNormals,
                          geometryType: geometryType,
                          allocator: allocator);
    let mdlMeshPtr = UnsafeMutableRawPointer(Unmanaged.passRetained(mdlMesh).toOpaque());
    return mdlMeshPtr;
}


// box with extent
//(boxWithExtent: <#T##vector_float3#>, segments: <#T##vector_uint3#>, inwardNormals: <#T##Bool#>, geometryType: <#T##MDLGeometryType#>, allocator: MDLMeshBufferAllocator?)

public func createBoxMDLMesh(
    boxWithExtentPtr: UnsafePointer<Float>,
    segmentsPtr: UnsafePointer<UInt32>,
    inwardNormals: Bool,
    rawGeometryType: Int,
    allocatorPtr: UnsafeMutableRawPointer
) -> UnsafeMutableRawPointer
{
    let allocator = Unmanaged<MTKMeshBufferAllocator>.fromOpaque(allocatorPtr).takeUnretainedValue();
    let geometryType = MDLGeometryType(rawValue: rawGeometryType)!;
    let mdlMesh = MDLMesh(boxWithExtent: pointerToSIMD(pointer: boxWithExtentPtr),
                          segments: pointerToSIMD(pointer: segmentsPtr),
                          inwardNormals: inwardNormals,
                          geometryType: geometryType,
                          allocator: allocator);
    let mdlMeshPtr = UnsafeMutableRawPointer(Unmanaged.passRetained(mdlMesh).toOpaque());
    return mdlMeshPtr;
}

public func setMDLMeshVertexDescriptor(mdlMeshPtr: UnsafeMutableRawPointer,
                                       mdlVertexDescriptorPtr: UnsafeMutableRawPointer)
{
    let mdlMesh = Unmanaged<MDLMesh>.fromOpaque(mdlMeshPtr).takeUnretainedValue();
    let mdlVertexDescriptor = Unmanaged<MDLVertexDescriptor>.fromOpaque(mdlVertexDescriptorPtr).takeUnretainedValue();
    mdlMesh.vertexDescriptor = mdlVertexDescriptor;
}

// release
public func releaseMDLMesh(mdlMeshPtr: UnsafeMutableRawPointer)
{
    let mdlMesh = Unmanaged<MDLMesh>.fromOpaque(mdlMeshPtr);
    mdlMesh.release();
}

// MTKMesh
// getVertexBuffers: [MTKMeshBuffer]
// getVertexDescriptor: MDLVertexDescriptor
// var submeshes: [MTKSubmesh]
// var vertexCount: Int
// c++ equivalents created on construction by calling private helpers

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

public func getMTKMeshVertexCount(mdlMeshPtr: UnsafeMutableRawPointer) -> Int
{
    let mdlMesh = Unmanaged<MDLMesh>.fromOpaque(mdlMeshPtr).takeUnretainedValue();
    return mdlMesh.vertexCount;
}

// MTKMeshBuffer
// MTLVertexBuffer*
// int offset
// get Vertex buffers
public func countVertexBuffers(
    mtkMeshPtr: UnsafeMutableRawPointer
) -> Int
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr).takeUnretainedValue();
    let vertexBuffers = mtkMesh.vertexBuffers;
    return vertexBuffers.count;
}

public func extractMeshBuffers
(
    mtkMeshPtr: UnsafeMutableRawPointer,
    mtkVertexBuffersPtr: UnsafeMutablePointer<UnsafeMutableRawPointer>,
    offsetsPtr: UnsafeMutablePointer<Int>
)
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr).takeUnretainedValue();
    let vertexBuffers = mtkMesh.vertexBuffers;
    for (index, vertexBuffer) in vertexBuffers.enumerated() {
        mtkVertexBuffersPtr[index] = UnsafeMutableRawPointer(Unmanaged.passUnretained(vertexBuffer.buffer).toOpaque());
        offsetsPtr[index] = vertexBuffer.offset;
    }
}

// MTKSubmesh
//var primitiveType: MTLPrimitiveType
//var indexType: MTLIndexType
//var indexBuffer: MTKMeshBuffer
//var indexCount: Int
public func countSubmeshes(
    mtkMeshPtr: UnsafeMutableRawPointer
) -> Int
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr).takeUnretainedValue();
    let submeshes = mtkMesh.submeshes;
    return submeshes.count;
}


public func extractSubmeshPrimitiveTypes(
    mtkMeshPtr: UnsafeMutableRawPointer,
    primitiveTypesPtr: UnsafeMutablePointer<UInt>
)
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr).takeUnretainedValue();
    let submeshBuffers = mtkMesh.submeshes;
    for (index, submesh) in submeshBuffers.enumerated() {
        primitiveTypesPtr[index] = submesh.primitiveType.rawValue;
    }
}

public func extractSubmeshIndexTypes(
    mtkMeshPtr: UnsafeMutableRawPointer,
    indexTypesPtr: UnsafeMutablePointer<UInt>
)
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr).takeUnretainedValue();
    let submeshBuffers = mtkMesh.submeshes;
    for (index, vertexBuffer) in submeshBuffers.enumerated() {
        indexTypesPtr[index] = vertexBuffer.indexType.rawValue;
    }
}

public func extractSubmeshIndexBuffers
(
    mtkMeshPtr: UnsafeMutableRawPointer,
    submeshBufffersPtr: UnsafeMutablePointer<UnsafeMutableRawPointer>,
    offsetsPtr: UnsafeMutablePointer<Int>
)
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr).takeUnretainedValue();
    let submeshBuffers = mtkMesh.submeshes;
    for (index, submesh) in submeshBuffers.enumerated() {
        submeshBufffersPtr[index] = UnsafeMutableRawPointer(Unmanaged.passUnretained(submesh.indexBuffer.buffer).toOpaque());
        offsetsPtr[index] = submesh.indexBuffer.offset;
    }
}

public func extractSubmeshIndexCounts
(
    mtkMeshPtr: UnsafeMutableRawPointer,
    submeshIndexCounts: UnsafeMutablePointer<Int>
)
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr).takeUnretainedValue();
    let submeshes = mtkMesh.submeshes;
    for (index, submesh) in submeshes.enumerated() {
        submeshIndexCounts[index] = submesh.indexCount;
    }
}

// release
public func releaseMTKMesh(mtkMeshPtr: UnsafeMutableRawPointer)
{
    let mtkMesh = Unmanaged<MTKMesh>.fromOpaque(mtkMeshPtr);
    mtkMesh.release();
}








