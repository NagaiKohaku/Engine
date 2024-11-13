#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "d3d12.h"

#include "string"
#include "list"
#include "unordered_map"
#include "random"
#include "wrl.h"

class DirectXCommon;

class TextureManager;

class SrvManager;

class Camera;

class ParticleManager {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	//マテリアル
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	//ローカル情報
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	//パーティクル
	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	//パーティクルGPU転送用データ
	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	//モデルのマテリアルデータ
	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	//AABB
	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	//加速場
	struct AccelerationField {
		Vector3 acceleration;
		AABB area;
	};

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
public:

	//パーティクルグループ
	struct ParticleGroup {
		MaterialData material;
		std::list<Particle> particles;
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
		uint32_t srvIndex;
		uint32_t numInstance;
		ParticleForGPU* instancingData;
		AccelerationField accelerationField;
		bool useAccelerationField;
	};

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//インスタンスの最大数
	static const uint32_t kNumMaxInstance;

	//1フレームで進む秒数
	static const float kDeltaTime;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルグループの生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャパス</param>
	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="position">座標</param>
	/// <param name="count">数</param>
	void Emit(const std::string name, const Vector3& position, uint32_t count);


	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// デフォルトカメラの設定
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

	/// <summary>
	/// 加速場のセッター
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="acceleration">加速度</param>
	void SetAcceleration(const std::string name, const Vector3& acceleration);

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetDefaultCamera() const { return defaultCamera_; }

	/// <summary>
	/// パーティクルグループのゲッター
	/// </summary>
	/// <returns>パーティクルグループ</returns>
	std::unordered_map<std::string, ParticleGroup> GetParticleGroup() { return particleGroups; }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// パイプラインの生成
	/// </summary>
	void CreatePipeLine();

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="point">座標</param>
	/// <returns>判定フラグ</returns>
	bool IsCollision(const AABB& aabb, const Vector3& point);

	/// <summary>
	/// 新しいパーティクルの生成
	/// </summary>
	/// <param name="translate">座標</param>
	/// <returns>パーティクル</returns>
	Particle MakeNewParticle(const Vector3& translate);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//DirectC基底
	DirectXCommon* directXCommon_;

	//テクスチャマネージャー
	TextureManager* textureManager_;

	//SRVマネージャー
	SrvManager* srvManager_;

	//デフォルトカメラ
	Camera* defaultCamera_;

	//ランダム
	std::mt19937 randomEngine_;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	//グラフィックパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	/// === バッファリソース === ///

	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//頂点番号リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexResource_ = nullptr;

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	/// === バッファリソース内のデータを指すポインタ === ///

	//頂点データ
	VertexData* vertexData_ = nullptr;

	//頂点番号データ
	uint32_t* indexData_ = nullptr;

	//マテリアルデータ
	Material* materialData_ = nullptr;

	/// === バッファビュー === ///

	//頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//頂点番号バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//パーティクルグループ
	std::unordered_map<std::string, ParticleGroup> particleGroups;

};