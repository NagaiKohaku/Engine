#pragma once
#include "BaseScene.h"

#include "Camera.h"
#include "Object3D.h"
#include "Object2D.h"
#include "Audio.h"

#include "memory"

class GameScene : public BaseScene {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	//音声データ
	SoundData soundData_;

	//箱
	std::unique_ptr<Object3D> cube_;

	//球
	std::unique_ptr<Object3D> ball_;

	//地面
	std::unique_ptr<Object3D> ground_;

	//SE
	SoundObject soundObject_;

};