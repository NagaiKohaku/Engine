#pragma once
#include "d3dx12.h"
#include "d3d12.h"

#include "wrl.h"
#include "vector"

#include "Vector3.h"

#include "DirectionalLight.h"
#include "PointLight.h"

class DirectXCommon;

class Camera;

///=====================================================/// 
/// 3Dオブジェクト基底
///=====================================================///
class Object3DCommon {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	//ブレンドモードの種類
	enum BlendType {
		Normal,   //通常
		Add,      //加算
		Subtruct, //減算
		Multily,  //乗算
		Screen    //スクリーン
	};

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
private:

	struct CameraForGPU {
		Vector3 worldPosition;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static Object3DCommon* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void CommonDrawSetting();

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// DirectX基底のゲッター
	/// </summary>
	/// <returns></returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns></returns>
	Camera* GetDefaultCamera() const { return defaultCamera_; }

	/// <summary>
	/// デフォルトカメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

	/// <summary>
	/// ブレンドモードのセッター
	/// </summary>
	/// <param name="blendType">ブレンドタイプ</param>
	void SetBlendMode(BlendType blendType) { blendMode_ = blendType; }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	void CreateGraphicsPipeline();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectX基底
	DirectXCommon* dxCommon_ = nullptr;

	//ブレンドモード
	BlendType blendMode_;

	//デフォルトカメラ
	Camera* defaultCamera_ = nullptr;

	//平行光源ライト
	std::unique_ptr<DirectionalLight> directionalLight_;

	//点光源ライト
	std::unique_ptr<PointLight> pointLight_;

	//バッファリソース内のデータを指すポインタ
	CameraForGPU* cameraForGpuData = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGpuResource = nullptr;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//グラフィックパイプラインステート
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> graphicsPipelineState_;
};