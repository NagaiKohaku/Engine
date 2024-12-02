#pragma once

#include "Vector3.h"
#include "Vector4.h"

#include "d3dx12.h"
#include "wrl.h"

class DirectXCommon;

///=====================================================/// 
/// 平行光源ライト
///=====================================================///
class DirectionalLight {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// データをGPUに送信
	/// </summary>
	void SendDataForGPU();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
private:

	//光源データ
	struct LightData {
		Vector4 color;     //色
		Vector3 direction; //向き
		float intensity;   //照度
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> lightResource_ = nullptr;

	//光源データ
	LightData* lightData_ = nullptr;
};