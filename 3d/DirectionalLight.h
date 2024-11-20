#pragma once

#include "Vector3.h"
#include "Vector4.h"

#include "d3dx12.h"
#include "wrl.h"

class DirectXCommon;

///=====================================================/// 
/// ���s�������C�g
///=====================================================///
class DirectionalLight {

	///-------------------------------------------/// 
	/// �����o�֐�
	///-------------------------------------------///
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �f�[�^��GPU�ɑ��M
	/// </summary>
	void SendDataForGPU();

	///-------------------------------------------/// 
	/// �\����
	///-------------------------------------------///
private:

	//�����f�[�^
	struct LightData {
		Vector4 color;     //�F
		Vector3 direction; //����
		float intensity;   //�Ɠx
	};

	///-------------------------------------------/// 
	/// �����o�ϐ�
	///-------------------------------------------///
private:

	//DirectX���
	DirectXCommon* dxCommon_ = nullptr;

	//�o�b�t�@���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> DirectionalLightResource_ = nullptr;

	//�����f�[�^
	LightData* directionalLightData_ = nullptr;
};