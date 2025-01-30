//
//  ModelIOAssets.swift
//  ModelIO_Bindings
//
//  Created by Taanish Reja on 1/29/25.
//
import Metal
import MetalKit
import ModelIO


public func createMDLAsset(filepath: UnsafeMutablePointer<CChar>,
                           mdlVertexDescriptorPtr: UnsafeMutableRawPointer,
                           bufferAllocatorPtr: UnsafeMutableRawPointer)
-> UnsafeMutableRawPointer?
{
    let mdlVertexDescriptor: MDLVertexDescriptor = Unmanaged<MDLVertexDescriptor>.fromOpaque(mdlVertexDescriptorPtr).takeUnretainedValue();
    let bufferAllocator: MTKMeshBufferAllocator = Unmanaged<MTKMeshBufferAllocator>.fromOpaque(bufferAllocatorPtr).takeUnretainedValue();
    
    let fileURL: URL = URL(fileURLWithPath: String(cString: filepath))
    let mdlAsset: MDLAsset = MDLAsset(url: fileURL, vertexDescriptor: mdlVertexDescriptor, bufferAllocator: bufferAllocator);
    
    mdlAsset.loadTextures();
    let mdlAssetPtr = Unmanaged.passUnretained(mdlAsset).toOpaque();
    return mdlAssetPtr;
}

public func mdlAssetMeshCount(mdlAssetPtr: UnsafeMutableRawPointer) -> Int {
    let mdlAsset: MDLAsset = Unmanaged<MDLAsset>.fromOpaque(mdlAssetPtr).takeUnretainedValue();
    let meshes: [MDLMesh] = mdlAsset.childObjects(of: MDLMesh.self) as! [MDLMesh];
    return meshes.count;

}
public func mdlAssetAsMesh(mdlAssetPtr: UnsafeMutableRawPointer,
                           meshesPtr: UnsafeMutablePointer<UnsafeMutableRawPointer>,
                           count: Int) {
    let mdlAsset: MDLAsset = Unmanaged<MDLAsset>.fromOpaque(mdlAssetPtr).takeUnretainedValue();
    let meshes: [MDLMesh] = mdlAsset.childObjects(of: MDLMesh.self) as! [MDLMesh];
    
    for (index, mesh) in meshes.prefix(count).enumerated() {
        meshesPtr[index] = Unmanaged.passUnretained(mesh).toOpaque();
    }
}

// TODO: Get asset textures (from submeshes?)

public func releaseMDLAsset(mdlAssetPtr: UnsafeMutableRawPointer) {
    let mdlAsset = Unmanaged<MDLAsset>.fromOpaque(mdlAssetPtr)
    mdlAsset.release();
}
