#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "WorldTransform.h"

#include "d3d12.h"

#include "vector"
#include "wrl.h"

class DebugObjectCommon;

class Camera;

///=====================================================/// 
/// デバッグライン
///=====================================================///
class DebugLine {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DebugLine();

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

	//頂点データ
	struct VertexData {
		Vector4 position;
	};

	//座標変換行列
	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

	//マテリアル
	struct Material {
		Vector4 color;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indexes;
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//デバッグオブジェクト基底
	DebugObjectCommon* debugCommon_;

	//カメラ
	Camera* camera_;

	//ワールドトランスフォーム
	WorldTransform transform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//リソースデータ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	TransformationMatrix* wvpData_ = nullptr;
	Material* materialData_ = nullptr;

	//バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//モデルデータ
	ModelData modelData_;
};