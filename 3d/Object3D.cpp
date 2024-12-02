#include "Object3D.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3DCommon.h"
#include "Model.h"
#include "ModelManager.h"
#include "Camera.h"
#include "imgui.h"

#include "MakeMatrixMath.h"

#include "cassert"

///=====================================================/// 
/// コンストラクタ
///=====================================================///
Object3D::Object3D() {

	//3Dオブジェクト基底のインスタンスを取得
	object3DCommon_ = Object3DCommon::GetInstance();

	//モデルの設定
	model_ = ModelManager::GetInstance()->FindModel("Default");

	//座標変換行列リソースを作成
	WVPResource_ = object3DCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得する
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//座標変換行列データの設定
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	//Transformの設定
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	//今持っているカメラをデフォルトカメラに設定
	camera_ = object3DCommon_->GetDefaultCamera();
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void Object3D::Update() {

	//ワールド行列
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	//ワールドビュープロジェクション行列
	Matrix4x4 worldViewProjectionMatrix = worldMatrix;

	if (camera_) {

		/// === カメラ情報があったら === ///

		//カメラのビュープロジェクション行列を取得
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		//ワールドビュープロジェクション行列の計算
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}

	//座標変換行列データの設定
	WVPData_->WVP = worldViewProjectionMatrix;
	WVPData_->World = worldMatrix;
	WVPData_->WorldInverseTranspose = Inverse4x4(worldMatrix);
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Object3D::Draw() {

	//座標変換行列データの設定
	object3DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

	//3Dモデルが割り当てられていれば描画する
	if (model_) {
		model_->Draw();
	}
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void Object3D::DisplayImGui() {

	Vector4 color = model_->GetColor();

	float shininess = model_->GetShininess();

	ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("Scale", &transform_.scale.x, 0.1f);
	ImGui::ColorEdit3("Color", &color.x);
	ImGui::DragFloat("Shininess", &shininess, 0.1f);

	model_->SetColor(color);
	model_->SetShininess(shininess);
}

///=====================================================/// 
/// モデルのセッター
///=====================================================///
void Object3D::SetModel(const std::string& modelName) {

	model_ = ModelManager::GetInstance()->FindModel(modelName);
}