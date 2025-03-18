#pragma once

#include "d3d12.h"

#include "wrl.h"

class DirectXCommon;

class PostProcess {

public:

	void Initialize();

private:

	void CreatePostProcessResource();

private:

	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr<ID3D12Resource> screenResource_;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> postProcessRTVHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> postProcessSRVHeap = nullptr;
};