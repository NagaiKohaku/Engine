#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"

class Shadow {

public:

	void Initialize();

	void Update();

	void Draw();

private:

	void CreateDSV();

	void CreateSRV();

	void CreateDepthStencilView();

	void CreateRootSignature();

	void CreatePipeLine();

private:

	//DSV
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> shadowDSV_;

	//SRV
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> shadowSRV_;

	//�[�x�X�e���V���r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> shadowDepthStencilView_;

	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> shadowRootSignature_ = nullptr;

	//�p�C�v���C��
	Microsoft::WRL::ComPtr<ID3D12PipelineState> shadowPipelineState_;

};