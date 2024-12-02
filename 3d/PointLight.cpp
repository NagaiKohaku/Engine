#include "PointLight.h"

#include "DirectXCommon.h"

#include "imgui.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void PointLight::Initialize() {

	//DirectX基底を取得
	dxCommon_ = DirectXCommon::GetInstance();

	//リソースを作成
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(LightData));

	//光源データのアドレスを記録
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));

	/// === 光源データの設定 === ///

	//色の設定
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };

	//座標の設定
	lightData_->position = { 0.0f,0.0f,0.0f };

	//照度の設定
	lightData_->intensity = 1.0f;

	//半径の設定
	lightData_->radius = 10.0f;

	//減少度の設定
	lightData_->decay = 1.0f;

}

///=====================================================/// 
/// 更新
///=====================================================///
void PointLight::Update() {
}

///=====================================================/// 
/// データをGPUに送信
///=====================================================///
void PointLight::SendDataForGPU() {

	//光源データをGPUに送信
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, lightResource_.Get()->GetGPUVirtualAddress());
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void PointLight::DisplayImGui() {

	ImGui::Begin("PointLight");

	ImGui::ColorEdit4("Color", &lightData_->color.x);

	ImGui::DragFloat3("Position", &lightData_->position.x, 0.1f);

	ImGui::DragFloat("Intensity", &lightData_->intensity, 0.01f);

	ImGui::DragFloat("Radius", &lightData_->radius, 0.01f);

	ImGui::DragFloat("Decay", &lightData_->decay, 0.01f);

	ImGui::End();
}
