#include "Object3D.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3DCommon.h"
#include "Model.h"
#include "ModelManager.h"
#include "Camera.h"
#include "DebugLine.h"
#include "Renderer.h"

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

	transform_.Initialize();

	//今持っているカメラをデフォルトカメラに設定
	camera_ = object3DCommon_->GetDefaultCamera();

	/// ===  === ///

	debugLines_.resize(3);

	debugLines_[0] = std::make_unique<DebugLine>();
	debugLines_[0]->Initialize(transform_.GetRight(), { 1.0f,0.0f,0.0f,1.0f });
	debugLines_[0]->SetScale({ 1.5f,1.5f,1.5f });
	debugLines_[0]->SetParent(&transform_);

	debugLines_[1] = std::make_unique<DebugLine>();
	debugLines_[1]->Initialize(transform_.GetUp(), { 0.0f,1.0f,0.0f,1.0f });
	debugLines_[1]->SetScale({ 1.5f,1.5f,1.5f });
	debugLines_[1]->SetParent(&transform_);

	debugLines_[2] = std::make_unique<DebugLine>();
	debugLines_[2]->Initialize(transform_.GetForward(), { 0.0f,0.0f,1.0f,1.0f });
	debugLines_[2]->SetScale({ 1.5f,1.5f,1.5f });
	debugLines_[2]->SetParent(&transform_);
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void Object3D::Update() {

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

	for (auto& line : debugLines_) {
		line->Update();
	}
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Object3D::Draw(LayerType layer) {

	std::function<void()> func;

	func = [this]() {

		//3Dオブジェクトの描画前処理
		object3DCommon_->CommonDrawSetting();

		//座標変換行列データの設定
		object3DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

		//3Dモデルが割り当てられていれば描画する
		if (model_) {
			model_->Draw();
		}
		};

	Renderer::GetInstance()->AddDraw(layer, func);

}

void Object3D::DebugDraw() {

	for (auto& line : debugLines_) {
		line->Draw();
	}
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void Object3D::DisplayImGui() {

	Vector4 color = model_->GetColor();

	float shininess = model_->GetShininess();

	transform_.DisplayImGui();

	ImGui::ColorEdit3("Color", &color.x);
	ImGui::DragFloat("Shininess", &shininess, 0.1f);

	model_->SetColor(color);
	model_->SetShininess(shininess);

	//if (ImGui::Button("Forward")) {
	//	debugLineX_->SetRotate(transform_.GetForward());
	//}
	//if (ImGui::Button("Up")) {
	//	debugLineX_->SetRotate(transform_.GetUp());
	//}
	//if (ImGui::Button("Rigth")) {
	//	debugLineX_->SetRotate(transform_.GetRight());
	//}
}

///=====================================================/// 
/// モデルのセッター
///=====================================================///
void Object3D::SetModel(const std::string& modelName) {

	model_ = ModelManager::GetInstance()->FindModel(modelName);
}