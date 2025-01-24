#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "WorldTransform.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "string"
#include "vector"
#include "wrl.h"

class Object3DCommon;

class ModelCommon;

class Camera;

class Line {

	///-------------------------------------------/// 
	/// �����o�֐�
	///-------------------------------------------///
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Line();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// �����o�\����
	///-------------------------------------------///
private:

	//���W�ϊ��s��f�[�^
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	//���_�f�[�^
	struct VertexData {
		Vector4  position;
		Vector2  texcoord;
		Vector3  normal;
	};

	//�}�e���A��
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	///-------------------------------------------/// 
	/// �����o�ϐ�
	///-------------------------------------------///
private:

	//3D�I�u�W�F�N�g���
	Object3DCommon* object3DCommon_;

	//���f�����
	ModelCommon* modelCommon_;

	//�J����
	Camera* camera_;

	//���W�f�[�^
	WorldTransform transform_;

	//�o�b�t�@���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	//�o�b�t�@���\�[�X���̃f�[�^���w���|�C���^
	TransformationMatrix* WVPData_ = nullptr;

	//�o�b�t�@���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

	//�o�b�t�@���\�[�X���̃f�[�^���w���|�C���^
	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;
	uint32_t* indexData_ = nullptr;

	//�o�b�t�@���\�[�X�̎g������⑫����o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
};

