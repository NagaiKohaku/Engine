#include "DirectionalLight.h"

#include "DirectXCommon.h"

#include "imgui.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void DirectionalLight::Initialize() {

	//DirectX基底を取得
	dxCommon_ = DirectXCommon::GetInstance();

	//リソースを作成
	DirectionalLightResource_ = dxCommon_->CreateBufferResource(sizeof(LightData));

	//光源データのアドレスを記録
	DirectionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	/// === 光源データの設定 === ///

	//色の設定
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };

	//向きの設定
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };

	//照度の設定
	directionalLightData_->intensity = 1.0f;
}

///=====================================================/// 
/// 更新
///=====================================================///
void DirectionalLight::Update() {

	//向きを正規化する
	directionalLightData_->direction = Normalize(directionalLightData_->direction);
}

///=====================================================/// 
/// データをGPUに送信
///=====================================================///
void DirectionalLight::SendDataForGPU() {

	//光源データをGPUに送信
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, DirectionalLightResource_.Get()->GetGPUVirtualAddress());
}

void DirectionalLight::DisplayImGui() {

	ImGui::Begin("DirectionalLight");

	ImGui::ColorEdit4("Color", &directionalLightData_->color.x);

	ImGui::DragFloat3("Direction", &directionalLightData_->direction.x, 0.01f, -1.0f, 1.0f);

	ImGui::DragFloat("Intensity", &directionalLightData_->intensity, 0.01f);

	ImGui::End();
}