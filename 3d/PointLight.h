#pragma once

#include "Vector3.h"
#include "Vector4.h"

#include "d3dx12.h"
#include "wrl.h"

class DirectXCommon;

///=====================================================/// 
/// 点光源ライト
///=====================================================///
class PointLight {

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
		Vector4 color;    //色
		Vector3 position; //座標
		float intensity;  //照度
		float radius;     //半径
		float decay;      //減少度
		float padding[2]; //穴埋め
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