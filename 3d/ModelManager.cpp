#include "ModelManager.h"
#include "ModelCommon.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void ModelManager::Initialize() {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//モデルデータ
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//球体モデルの作成とテクスチャの読み込み
	model->InitializeSphere("Resource/Texture/", "white_128x128.png");

	//モデル名とモデルデータをコンテナに登録
	models_.insert(std::make_pair("Default", std::move(model)));
}

///=====================================================/// 
/// モデルの読み込み
///=====================================================///
void ModelManager::LoadModel(const std::string& modelName, const std::string& modelFileName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		//読み込み済みなら早期return
		return;
	}

	//モデルデータ
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//モデルの読み込み
	model->Initialize("Resource/Model/" + modelFileName, modelFileName + ".obj");

	//モデル名とモデルデータをコンテナに登録
	models_.insert(std::make_pair(modelName, std::move(model)));
}

///=====================================================/// 
/// 球体モデルの生成
///=====================================================///
void ModelManager::CreateSphere(const std::string& modelName, const std::string& textureFileName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		//読み込み済みなら早期return
		return;
	}

	//モデルデータ
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//球体モデルの作成とテクスチャの読み込み
	model->InitializeSphere("Resource/Texture/", textureFileName + ".png");

	//モデル名とモデルデータをコンテナに登録
	models_.insert(std::make_pair(modelName, std::move(model)));
}

void ModelManager::CreateCube(const std::string& modelName, const std::string& textureFileName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		//読み込み済みなら早期return
		return;
	}

	//モデルデータ
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//球体モデルの作成とテクスチャの読み込み
	model->InitializeCube("Resource/Texture", textureFileName + ".png");

	//モデル名とモデルデータをコンテナに登録
	models_.insert(std::make_pair(modelName, std::move(model)));
}

///=====================================================/// 
/// モデルの検索
///=====================================================///
Model* ModelManager::FindModel(const std::string& modelName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		//読み込みモデルを戻り値としてreturn
		return models_.at(modelName).get();
	}

	//ファイル名一致なし
	return nullptr;
}