#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

class Object3D;

///=====================================================/// 
/// カメラ
///=====================================================///
class Camera {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// 追従対象のセッター
	/// </summary>
	/// <param name="object">3Dオブジェクト</param>
	void SetTrackingObject(Object3D* object) { object_ = object; }

	/// <summary>
	/// 追従対象のリセット
	/// </summary>
	void ResetTrackingObject() { object_ = nullptr; }

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

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
	/// デバッグカメラの角度のセッター
	/// </summary>
	/// <param name="rotate">角度</param>
	void SetDebugRotate(const Vector3& rotate) { debugCameraTransform_.rotate = rotate; }

	/// <summary>
	/// デバッグカメラの座標のセッター
	/// </summary>
	/// <param name="translate">座標</param>
	void SetDebugTranslate(const Vector3& translate) { debugCameraTransform_.translate = translate; }

	/// <summary>
	/// fovYのセッター
	/// </summary>
	/// <param name="fovY">fovY</param>
	void SetFovY(const float fovY) { fovY_ = fovY; }

	/// <summary>
	/// アスペクト比のセッター
	/// </summary>
	/// <param name="aspectRatio">アスペクト比</param>
	void SetAspectRatio(const float aspectRatio) { aspectRatio_ = aspectRatio; }

	/// <summary>
	/// NearClipのセッター
	/// </summary>
	/// <param name="nearClip">NearClip</param>
	void SetNearClip(const float nearClip) { nearClip_ = nearClip; }

	/// <summary>
	/// FarClipのセッター
	/// </summary>
	/// <param name="farClip">FarClip</param>
	void SetFarClip(const float farClip) { farClip_ = farClip; }

	/// <summary>
	/// デバッグカメラフラグのセッター
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetDebugCameraFlag(const bool flag) { isDebugCamera_ = flag; }

	/// <summary>
	/// 角度のゲッター
	/// </summary>
	/// <returns>角度</returns>
	const Vector3& GetRotate() const { return transform_.rotate; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetTranslate() const { return transform_.translate; }

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns>ワールド座標</returns>
	const Vector3& GetWorldTranslate() const { return { worldMatrix_.m[3][0],worldMatrix_.m[3][1],worldMatrix_.m[3][2] }; }

	/// <summary>
	/// ワールド行列のゲッター
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }

	/// <summary>
	/// ビュー行列のゲッター
	/// </summary>
	/// <returns>ビュー行列</returns>
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// プロジェクション行列のゲッター
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }

	/// <summary>
	/// ビュープロジェクション行列のゲッター
	/// </summary>
	/// <returns>ビュープロジェクション行列</returns>
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	//座標変換データ
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//追従対象
	Object3D* object_;

	//座標
	Transform transform_;

	//デバッグカメラの座標
	Transform debugCameraTransform_;

	//ワールド行列
	Matrix4x4 worldMatrix_;

	//ビュー行列
	Matrix4x4 viewMatrix_;

	//プロジェクション行列
	Matrix4x4 projectionMatrix_;

	//ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

	//視野角
	float fovY_;

	//アスペクト比
	float aspectRatio_;

	//NearClip
	float nearClip_;

	//FarClip
	float farClip_;

	//Z軸のオフセット
	float offsetZ_;

	//デバッグカメラのZ軸のオフセット
	float debugCameraOffsetZ_;

	//デバッグカメラフラグ
	bool isDebugCamera_;

};