#pragma once

#include "Vector3.h"

#include "cstdint"

///=====================================================/// 
/// パーティクルエミッター
///=====================================================///
class ParticleEmitter {

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//1フレームで進む秒数
	static const float kDeltaTime;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleEmitter();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルを生成
	/// </summary>
	void Emit();

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//ローカル情報
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
private:

	//座標
	Transform transform_;

	//数
	uint32_t count_;

	//発生間隔
	float frequency_;

	//発生タイマー
	float frequencyTime_;

};