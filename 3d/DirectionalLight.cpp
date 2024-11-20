#include "DirectionalLight.h"

#include "DirectXCommon.h"

///=====================================================/// 
/// ������
///=====================================================///
void DirectionalLight::Initialize() {

	//DirectX�����擾
	dxCommon_ = DirectXCommon::GetInstance();

	//���\�[�X���쐬
	DirectionalLightResource_ = dxCommon_->CreateBufferResource(sizeof(LightData));

	//�����f�[�^�̃A�h���X���L�^
	DirectionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	/// === �����f�[�^�̐ݒ� === ///

	//�F�̐ݒ�
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };

	//�����̐ݒ�
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };

	//�Ɠx�̐ݒ�
	directionalLightData_->intensity = 1.0f;
}

///=====================================================/// 
/// �X�V
///=====================================================///
void DirectionalLight::Update() {

	//�����𐳋K������
	directionalLightData_->direction = Normalize(directionalLightData_->direction);
}

///=====================================================/// 
/// �f�[�^��GPU�ɑ��M
///=====================================================///
void DirectionalLight::SendDataForGPU() {

	//�����f�[�^��GPU�ɑ��M
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, DirectionalLightResource_.Get()->GetGPUVirtualAddress());
}
