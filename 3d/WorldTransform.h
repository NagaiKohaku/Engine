#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

///=====================================================/// 
/// ワールドトランスフォーム
///=====================================================///
class WorldTransform {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// パラメータのデバッグ表示
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetTranslate() const { return translate_; }

	/// <summary>
	/// 角度のゲッター
	/// </summary>
	/// <returns>角度</returns>
	const Vector3& GetRotate() const { return rotate_; }

	/// <summary>
	/// 大きさのゲッター
	/// </summary>
	/// <returns>大きさ</returns>
	const Vector3& GetScale() const { return scale_; }

	/// <summary>
	/// ワールド行列のゲッター
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="vector">座標</param>
	void SetTranslate(const Vector3& vector) { translate_ = vector; }

	/// <summary>
	/// 角度のセッター
	/// </summary>
	/// <param name="vector">角度</param>
	void SetRotate(const Vector3& vector) { rotate_ = vector; }

	/// <summary>
	/// 大きさのセッター
	/// </summary>
	/// <param name="vector">大きさ</param>
	void SetScale(const Vector3& vector) { scale_ = vector; }

	/// <summary>
	/// ワールド行列のセッター
	/// </summary>
	/// <param name="matrix">ワールド行列</param>
	void SetWorldMatrix(const Matrix4x4& matrix) { worldMatrix_ = matrix; }

	/// <summary>
	/// オフセットのセッター
	/// </summary>
	/// <param name="vector">オフセット</param>
	void SetOffset(const Vector3& vector) { offset_ = vector; }

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns>ワールド座標</returns>
	const Vector3& GetWorldTranslate() const;

	/// <summary>
	/// 親オブジェクトのセッター
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

private:

	//座標
	Vector3 translate_;

	//角度
	Vector3 rotate_;

	//大きさ
	Vector3 scale_;

	//ワールド行列
	Matrix4x4 worldMatrix_;

	//オフセット
	Vector3 offset_;

	//親オブジェクト
	WorldTransform* parent_;
};