#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

#include "Vector3.h"

class DirectXCommon;

class DebugObjectCommon {

	///-------------------------------------------/// 
	/// �����o�֐�
	///-------------------------------------------///
public:

	/// <summary>
	/// �V���O���g���C���X�^���X
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static DebugObjectCommon* GetInstance();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��O����
	/// </summary>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// �Z�b�^�[�E�Q�b�^�[
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectX���̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	///-------------------------------------------/// 
	/// �N���X�������֐�
	///-------------------------------------------///
private:

	/// <summary>
	/// ���[�g�V�O�l�`���̐���
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// �����o�ϐ�
	///-------------------------------------------///
private:

	//DirectX���
	DirectXCommon* dxCommon_ = nullptr;

	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//�O���t�B�b�N�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
};