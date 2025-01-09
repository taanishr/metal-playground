//
//  ModelIOAllocators.swift
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/8/25.
//

import Metal
import MetalKit
import ModelIO

public func createMTKMeshBufferAllocator(devicePtr: UnsafeMutableRawPointer) -> UnsafeMutableRawPointer
{
    let device = Unmanaged<MTLDevice>.fromOpaque(devicePtr).takeUnretainedValue();
    
    let mtkMeshBufferAllocator = MTKMeshBufferAllocator(device: device);
    
    let mtkMeshBufferAllocatorPtr = UnsafeMutableRawPointer(Unmanaged.passRetained(mtkMeshBufferAllocator).toOpaque());
    
    return mtkMeshBufferAllocatorPtr;
}

public func releaseMTKMeshBufferAllocator(mtkMeshBufferAllocatorPtr: UnsafeMutableRawPointer)
{
    let mtkMeshBufferAllocator = Unmanaged<MTKMeshBufferAllocator>.fromOpaque(mtkMeshBufferAllocatorPtr);
    mtkMeshBufferAllocator.release();
}
