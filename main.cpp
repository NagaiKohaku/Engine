#include "WinApp.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "SpriteCommon.h"
#include "Object3DCommon.h"
#include "Object2DCommon.h"
#include "ModelCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleCommon.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Audio.h"
#include "ImGuiManager.h"
#include "SceneManager.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	///-------------------------------------------/// 
	/// エンジン初期化
	///-------------------------------------------///

	//ウィンドウ
	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	//DirectX基底
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize();

	//ImGuiマネージャー
	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize();

	//SRVマネージャー
	SrvManager* srvManager = SrvManager::GetInstance();
	srvManager->Initialize();

	//スプライト基底
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize();

	//3Dオブジェクト基底
	Object3DCommon* object3DCommon = Object3DCommon::GetInstance();
	object3DCommon->Initialize();

	Object2DCommon* object2dCommon = Object2DCommon::GetInstance();
	object2dCommon->Initialize();

	//モデル基底
	ModelCommon* modelCommon = ModelCommon::GetInstance();
	modelCommon->Initialize();

	ParticleCommon* particleCommon = ParticleCommon::GetInstance();
	particleCommon->Initialize();

	//パーティクルマネージャー
	ParticleManager* particleManager = ParticleManager::GetInstance();
	particleManager->Initialize();

	//入力
	Input* input = Input::GetInstance();
	input->Initialize();

	//音声
	Audio* audio = Audio::GetInstance();
	audio->Initialize();

	//シーンマネージャー
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Initialize();

	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize();

	//モデルマネージャー
	ModelManager::GetInstance()->Initialize();

	//シーンを設定
	sceneManager->ChangeScene(SceneManager::kGame);

	///-------------------------------------------/// 
	/// メインループ
	///-------------------------------------------///

	while (true) {

		//ウィンドウから終了メッセージが来たらループを抜ける
		if (winApp->ProcessMessage()) {
			break;
		}

		///-------------------------------------------/// 
		/// 更新処理
		///-------------------------------------------///

		//ImGuiの受付開始
		imGuiManager->Begin();

		//入力の更新
		input->Update();

		//3dオブジェクト基底の更新
		object3DCommon->Update();

		//シーンの更新
		sceneManager->Update();

		//パーティクルの更新
		particleManager->Update();

		//ImGuiの受付終了
		imGuiManager->End();

		///-------------------------------------------/// 
		/// 描画処理
		///-------------------------------------------///

		//DirectX基底の描画前処理
		directXCommon->PreDraw();

		//SRVマネージャーの描画前処理
		srvManager->PreDraw();

		//シーンの描画
		sceneManager->Draw();

		//パーティクルの描画
		particleManager->Draw();

		//ImGuiの描画
		imGuiManager->Draw();

		//DirectX基底の描画後処理
		directXCommon->PostDraw();

	}

	///-------------------------------------------/// 
	/// 終了処理
	///-------------------------------------------///

	audio->Finalize();

	imGuiManager->Finalize();

	winApp->Finalize();

	return 0;
}