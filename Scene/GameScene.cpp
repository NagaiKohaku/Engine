#include "GameScene.h"

#include "DirectXCommon.h"
#include "Object3DCommon.h"
#include "Object2DCommon.h"
#include "ModelManager.h"
#include "SpriteManager.h"
#include "ParticleManager.h"

#include "imgui.h"

#include "numbers"

void GameScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	camera_->SetDebugCameraFlag(true);

	//カメラの座標
	camera_->GetWorldTransform().SetTranslate({ 0.0f,3.0f,0.0f });

	//デフォルトカメラを設定
	Object3DCommon::GetInstance()->SetDefaultCamera(camera_.get());

	ParticleManager::GetInstance()->SetDefaultCamera(camera_.get());

	/// === リソースの読み込み === ///

	//スプライトのロード
	SpriteManager::GetInstance()->LoadSprite("Title", "RockShotTitle");

	//モデルのロード
	ModelManager::GetInstance()->LoadModel("Ground", "terrain");

	//音声データの読み込み
	soundData_ = Audio::GetInstance()->SoundLoad("Resource/Sound/SE/se.wav");

	/// === オブジェクトの生成 === ///

	/// === タイトルの生成 === ///

	/// === 箱の生成 === ///

	//箱の生成
	cube_ = std::make_unique<Object3D>();

	//座標の設定
	cube_->GetWorldTransform().SetTranslate({ 0.0f,1.0f,0.0f });

	//モデルの設定
	cube_->SetModel("Cube");

	//モデルの色を指定
	cube_->GetModel()->SetColor({ 0.5f,0.0f,0.0f,1.0f });

	/// === 球の生成 === ///

	//球の生成
	ball_ = std::make_unique<Object3D>();

	//座標の設定
	ball_->GetWorldTransform().SetTranslate({ 0.0f,3.0f,0.0f });

	//角度の設定
	ball_->GetWorldTransform().SetRotate({ 0.0f,static_cast<float>(std::numbers::pi) / 180.0f * -90.0f,0.0f });

	//モデルの設定
	ball_->SetModel("Sphere");

	//モデルの色の指定
	ball_->GetModel()->SetColor({ 0.5f,0.f,0.0f,1.0f });

	camera_->SetTrackingObject(ball_.get());

	/// === 地面の生成 === ///

	//地面の生成
	ground_ = std::make_unique<Object3D>();

	//角度の設定
	ground_->GetWorldTransform().SetRotate({ 0.0f,static_cast<float>(std::numbers::pi) / 180.0f * -90.0f,0.0f });

	//モデルの設定
	ground_->SetModel("Ground");

	/// === SEの生成 === ///

	soundObject_ = Audio::GetInstance()->CreateSoundObject(soundData_, false);

	ParticleManager::GetInstance()->CreateParticleGroup("Particle", "star.png");

	ParticleManager::GetInstance()->SetAcceleration("Particle", Vector3(0.0f, 5.0f, 0.0f), AABB({ -1.0f,-1.0f,-1.0f }, { 1.0f,1.0f,1.0f }));
}

void GameScene::Finalize() {

	//音声データの解放
	Audio::GetInstance()->SoundUnLoad(&soundData_);
}

void GameScene::Update() {

	//カメラをデバッグ状態で更新
	camera_->Update();

	//3Dオブジェクトの更新
	cube_->Update();

	ball_->Update();

	ground_->Update();

	ParticleManager::GetInstance()->Emit(
		"Particle",
		Vector3(0.0f, 0.0f, 0.0f),
		AABB({ -1.0f,0.0f,-1.0f }, { 1.0f,0.0f,1.0f }),
		Vector3(-1.0f, -2.0f, -1.0f),
		Vector3(1.0f, -1.0f, 1.0f),
		1.0f,
		3.0f,
		true,
		2
	);

	//ImGuiを起動
	ImGui::Begin("Scene");

	//モデルのImGui
	if (ImGui::TreeNode("Cube")) {

		cube_->DisplayImGui();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Ball")) {

		ball_->DisplayImGui();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Camera")) {

		camera_->DisplayImGui();

		ImGui::TreePop();
	}

	if (ImGui::Button("Start Audio")) {

		//音声データの再生
		Audio::GetInstance()->StartSound(soundObject_);
	}

	ImGui::Text("Shift + LeftClick : Move Camera");
	ImGui::Text("Shift + RightClick : Rotate Camera");
	ImGui::Text("Shift + MiddleWheel : Move Offset Camera");

	//ImGuiの終了
	ImGui::End();

}

void GameScene::Draw() {

	/// === 背景Spriteの描画 === ///

	//Spriteの描画準備
	Object2DCommon::GetInstance()->CommonDrawSetting();

	//深度情報のリセット
	DirectXCommon::GetInstance()->ClearDepthBuffer();

	/// === 3DObjectの描画 === ///

	//3DObjectの描画準備
	Object3DCommon::GetInstance()->CommonDrawSetting();

	////Object3Dの描画
	cube_->Draw();

	ball_->Draw();

	ground_->Draw();

	/// === 前景Spriteの描画 === ///

	Object2DCommon::GetInstance()->CommonDrawSetting();

}