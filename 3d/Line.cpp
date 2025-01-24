#include "Line.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3DCommon.h"
#include "ModelCommon.h"
#include "Camera.h"
#include "imgui.h"

#include "MakeMatrixMath.h"

#include "cassert"
Line::Line() {

	//3D�I�u�W�F�N�g���̃C���X�^���X���擾
	object3DCommon_ = Object3DCommon::GetInstance();

	//���f�����̃C���X�^���X���擾
	modelCommon_ = ModelCommon::GetInstance();

	//���W�ϊ��s�񃊃\�[�X���쐬
	WVPResource_ = object3DCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//�������ނ��߂̃A�h���X���擾����
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//���W�ϊ��s��f�[�^�̐ݒ�
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	transform_.Initialize();

	//�������Ă���J�������f�t�H���g�J�����ɐݒ�
	camera_ = object3DCommon_->GetDefaultCamera();

	/// === ���_���\�[�X === ///

	vertexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 2);

	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 2);

	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	/// === �C���f�b�N�X���\�[�X === ///

	indexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 2);

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 2);

	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	/// === �}�e���A�����\�[�X === ///

	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;
}

void Line::Update() {

	transform_.UpdateMatrix();

	//���[���h�r���[�v���W�F�N�V�����s��
	Matrix4x4 worldViewProjectionMatrix = transform_.GetWorldMatrix();

	if (camera_) {

		/// === �J������񂪂������� === ///

		//�J�����̃r���[�v���W�F�N�V�����s����擾
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		//���[���h�r���[�v���W�F�N�V�����s��̌v�Z
		worldViewProjectionMatrix *= viewProjectionMatrix;
	}

	//���W�ϊ��s��f�[�^�̐ݒ�
	WVPData_->WVP = worldViewProjectionMatrix;
	WVPData_->World = transform_.GetWorldMatrix();
	WVPData_->WorldInverseTranspose = Inverse4x4(transform_.GetWorldMatrix());
}

void Line::Draw() {

	//���W�ϊ��s��f�[�^�̐ݒ�
	object3DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

}
