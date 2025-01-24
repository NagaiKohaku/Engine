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

	//3Dオブジェクト基底のインスタンスを取得
	object3DCommon_ = Object3DCommon::GetInstance();

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//座標変換行列リソースを作成
	WVPResource_ = object3DCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得する
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//座標変換行列データの設定
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	transform_.Initialize();

	//今持っているカメラをデフォルトカメラに設定
	camera_ = object3DCommon_->GetDefaultCamera();

	/// === 頂点リソース === ///

	vertexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 2);

	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 2);

	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	/// === インデックスリソース === ///

	indexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 2);

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 2);

	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	/// === マテリアルリソース === ///

	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;
}

void Line::Update() {

	transform_.UpdateMatrix();

	//ワールドビュープロジェクション行列
	Matrix4x4 worldViewProjectionMatrix = transform_.GetWorldMatrix();

	if (camera_) {

		/// === カメラ情報があったら === ///

		//カメラのビュープロジェクション行列を取得
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		//ワールドビュープロジェクション行列の計算
		worldViewProjectionMatrix *= viewProjectionMatrix;
	}

	//座標変換行列データの設定
	WVPData_->WVP = worldViewProjectionMatrix;
	WVPData_->World = transform_.GetWorldMatrix();
	WVPData_->WorldInverseTranspose = Inverse4x4(transform_.GetWorldMatrix());
}

void Line::Draw() {

	//座標変換行列データの設定
	object3DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

}
