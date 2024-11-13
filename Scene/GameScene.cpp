#include "GameScene.h"

#include "numbers"

void GameScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	//カメラの座標
	camera_->SetTranslate({ 0.0f,0.0f,0.0f });

	//カメラの角度
	camera_->SetRotate({ 0.0f,static_cast<float>(std::numbers::pi) / 180.0f * 180.0f,0.0f });

	//デフォルトカメラを設定
	Object3DCommon::GetInstance()->SetDefaultCamera(camera_.get());

	/// === リソースの読み込み === ///

	//モデルのロード
	ModelManager::GetInstance()->CreateCube("MonsterCube", "monsterBall");

	ModelManager::GetInstance()->CreateSphere("MonsterBall", "monsterBall");

	ModelManager::GetInstance()->LoadModel("Ground", "terrain");

	//音声データの読み込み
	soundData_ = Audio::GetInstance()->SoundLoad("Resource/Sound/SE/se.wav");

	/// === オブジェクトの生成 === ///

	/// === 箱の生成 === ///

	//箱の生成
	cube_ = std::make_unique<Object3D>();

	//座標の設定
	cube_->SetTranslate({ 0.0f,1.0f,0.0f });

	//モデルの設定
	cube_->SetModel("MonsterCube");

	/// === 球の生成 === ///

	//球の生成
	ball_ = std::make_unique<Object3D>();

	//座標の設定
	ball_->SetTranslate({ 0.0f,3.0f,0.0f });

	//角度の設定
	ball_->SetRotate({ 0.0f,static_cast<float>(std::numbers::pi) / 180.0f * 90.0f,0.0f });

	//モデルの設定
	ball_->SetModel("MonsterBall");

	//カメラの追従対象に設定
	camera_->SetTrackingObject(ball_.get());

	/// === 地面の生成 === ///

	//地面の生成
	ground_ = std::make_unique<Object3D>();

	//角度の設定
	ground_->SetRotate({ 0.0f,static_cast<float>(std::numbers::pi) / 180.0f * 90.0f,0.0f });

	//モデルの設定
	ground_->SetModel("Ground");

	/// === SEの生成 === ///

	soundObject_ = Audio::GetInstance()->CreateSoundObject(soundData_, false);
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

	//ImGuiの終了
	ImGui::End();

}

void GameScene::Draw() {

	/// === Spriteの描画 === ///

	//Spriteの描画準備
	SpriteCommon::GetInstance()->CommonDrawSetting();

	/// === 3DObjectの描画 === ///

	//3DObjectの描画準備
	Object3DCommon::GetInstance()->CommonDrawSetting();

	//Object3Dの描画
	cube_->Draw();

	ball_->Draw();

	ground_->Draw();

	/// === Spriteの描画 === ///

	SpriteCommon::GetInstance()->CommonDrawSetting();

}