#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "DirectXTex.h"
#include "d3d12.h"

#include "string"
#include "vector"
#include "wrl.h"

class Object3DCommon;

class Model;

class Camera;

///=====================================================/// 
/// 3Dオブジェクト
///=====================================================///
class Object3D {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3D();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// モデルのセッター
	/// </summary>
	/// <param name="modelName">ファイル名</param>
	void SetModel(const std::string& filePath);

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// 大きさのセッター
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	/// <summary>
	/// 角度のセッター
	/// </summary>
	/// <param name="rotate">角度</param>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="translate">座標</param>
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }

	/// <summary>
	/// 大きさのゲッター
	/// </summary>
	/// <returns>大きさ</returns>
	Vector3 GetScale() const { return transform_.scale; }

	/// <summary>
	/// 角度のゲッター
	/// </summary>
	/// <returns>角度</returns>
	Vector3 GetRotate() const { return transform_.rotate; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns>座標</returns>
	Vector3 GetTranslate() const { return transform_.translate; }

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//座標変換行列データ
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	//座標変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	//平行光源
	struct DirectionalLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//3Dオブジェクト基底
	Object3DCommon* object3DCommon_;

	//カメラ
	Camera* camera_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> DirectionalLightResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* WVPData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	//座標データ
	Transform transform_;

	//モデル情報
	Model* model_;
};