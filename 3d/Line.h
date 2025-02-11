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
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Line();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

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

	//頂点データ
	struct VertexData {
		Vector4  position;
		Vector2  texcoord;
		Vector3  normal;
	};

	//マテリアル
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//3Dオブジェクト基底
	Object3DCommon* object3DCommon_;

	//モデル基底
	ModelCommon* modelCommon_;

	//カメラ
	Camera* camera_;

	//座標データ
	WorldTransform transform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* WVPData_ = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;
	uint32_t* indexData_ = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
};

