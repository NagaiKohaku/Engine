#include "Model.h"
#include "DirectXCommon.h"
#include "ModelCommon.h"
#include "TextureManager.h"

#include "MakeMatrixMath.h"

#include "fstream"
#include "sstream"
#include "numbers"

///=====================================================/// 
/// 初期化処理
///=====================================================///
void Model::Initialize(const std::string& directoryPath, const std::string& filename) {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//モデルデータの読み込み
	modelData_ = LoadObjFile(directoryPath, filename);

	//頂点リソースを作成
	vertexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	//マテリアルリソースを作成
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//リソースの先頭のアドレスを取得する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得する
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//頂点データの設定
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

	//マテリアルの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
}

///=====================================================/// 
/// 球体モデルの初期化処理
///=====================================================///
void Model::InitializeSphere(const std::string& directoryPath, const std::string& filename) {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//球体モデルの生成
	CreateSphereModel();

	//頂点リソースを作成
	vertexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	//マテリアルリソースを作成
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//リソースの先頭のアドレスを取得する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得する
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//頂点データの設定
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

	//マテリアルのファイルパスを設定
	modelData_.material.textureFilePath = directoryPath + "/" + filename;

	//マテリアルの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
}

void Model::InitializeCube(const std::string& directoryPath, const std::string& filename) {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//立方体モデルの生成
	CreateCubeModel();

	//頂点リソースを作成
	vertexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	//マテリアルリソースを作成
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//リソースの先頭のアドレスを取得する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得する
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//頂点データの設定
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

	//マテリアルのファイルパスを設定
	modelData_.material.textureFilePath = directoryPath + "/" + filename;

	//マテリアルの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Model::Draw() {

	//頂点データの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//マテリアルデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	//テクスチャデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));

	//描画コマンド発行
	modelCommon_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);

}

///=====================================================/// 
/// 球体モデルの生成
///=====================================================///
void Model::CreateSphereModel() {

	//分割数
	const uint32_t kSubDivision = 16;

	//経度1つ分の角度 φ
	const float kLonEvery = static_cast<float>(std::numbers::pi) * 2.0f / float(kSubDivision);

	//緯度の1つ分の角度 θ
	const float kLatEvery = static_cast<float>(std::numbers::pi) / float(kSubDivision);

	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex) {

		float lat = -static_cast<float>(std::numbers::pi) / 2.0f + kLatEvery * latIndex; //θ

		//緯度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {

			float lon = lonIndex * kLonEvery; //φ
			uint32_t start = (latIndex * kSubDivision + lonIndex) * 6;

			VertexData vertexData[6];

			//基準点a 左下
			vertexData[0].position.x = cosf(lat) * cosf(lon);
			vertexData[0].position.y = sinf(lat);
			vertexData[0].position.z = cosf(lat) * sinf(lon);
			vertexData[0].position.w = 1.0f;
			vertexData[0].texcoord = {
				float(lonIndex) / float(kSubDivision),
				1.0f - float(latIndex) / float(kSubDivision)
			};
			vertexData[0].normal.x = vertexData[0].position.x;
			vertexData[0].normal.y = vertexData[0].position.y;
			vertexData[0].normal.z = vertexData[0].position.z;

			//基準点b 左上
			vertexData[1].position.x = cosf(lat + kLatEvery) * cosf(lon);
			vertexData[1].position.y = sinf(lat + kLatEvery);
			vertexData[1].position.z = cosf(lat + kLatEvery) * sinf(lon);
			vertexData[1].position.w = 1.0f;
			vertexData[1].texcoord = {
				float(lonIndex) / float(kSubDivision),
				1.0f - float(latIndex + 1) / float(kSubDivision)
			};
			vertexData[1].normal.x = vertexData[1].position.x;
			vertexData[1].normal.y = vertexData[1].position.y;
			vertexData[1].normal.z = vertexData[1].position.z;

			//基準点c 右下
			vertexData[2].position.x = cosf(lat) * cosf(lon + kLonEvery);
			vertexData[2].position.y = sinf(lat);
			vertexData[2].position.z = cosf(lat) * sinf(lon + kLonEvery);
			vertexData[2].position.w = 1.0f;
			vertexData[2].texcoord = {
				float(lonIndex + 1) / float(kSubDivision),
				1.0f - float(latIndex) / float(kSubDivision)
			};
			vertexData[2].normal.x = vertexData[2].position.x;
			vertexData[2].normal.y = vertexData[2].position.y;
			vertexData[2].normal.z = vertexData[2].position.z;

			//基準点b 左上
			vertexData[3].position.x = cosf(lat + kLatEvery) * cosf(lon);
			vertexData[3].position.y = sinf(lat + kLatEvery);
			vertexData[3].position.z = cosf(lat + kLatEvery) * sinf(lon);
			vertexData[3].position.w = 1.0f;
			vertexData[3].texcoord = {
				float(lonIndex) / float(kSubDivision),
				1.0f - float(latIndex + 1) / float(kSubDivision)
			};
			vertexData[3].normal.x = vertexData[3].position.x;
			vertexData[3].normal.y = vertexData[3].position.y;
			vertexData[3].normal.z = vertexData[3].position.z;

			//基準点d 右上
			vertexData[4].position.x = cosf(lat + kLatEvery) * cosf(lon + kLonEvery);
			vertexData[4].position.y = sinf(lat + kLatEvery);
			vertexData[4].position.z = cosf(lat + kLatEvery) * sinf(lon + kLonEvery);
			vertexData[4].position.w = 1.0f;
			vertexData[4].texcoord = {
				float(lonIndex + 1) / float(kSubDivision),
				1.0f - float(latIndex + 1) / float(kSubDivision)
			};
			vertexData[4].normal.x = vertexData[4].position.x;
			vertexData[4].normal.y = vertexData[4].position.y;
			vertexData[4].normal.z = vertexData[4].position.z;

			//基準点c 右下
			vertexData[5].position.x = cosf(lat) * cosf(lon + kLonEvery);
			vertexData[5].position.y = sinf(lat);
			vertexData[5].position.z = cosf(lat) * sinf(lon + kLonEvery);
			vertexData[5].position.w = 1.0f;
			vertexData[5].texcoord = {
				float(lonIndex + 1) / float(kSubDivision),
				1.0f - float(latIndex) / float(kSubDivision)
			};
			vertexData[5].normal.x = vertexData[5].position.x;
			vertexData[5].normal.y = vertexData[5].position.y;
			vertexData[5].normal.z = vertexData[5].position.z;

			modelData_.vertices.push_back(vertexData[0]);
			modelData_.vertices.push_back(vertexData[1]);
			modelData_.vertices.push_back(vertexData[2]);
			modelData_.vertices.push_back(vertexData[3]);
			modelData_.vertices.push_back(vertexData[4]);
			modelData_.vertices.push_back(vertexData[5]);
		}
	}

}

void Model::CreateCubeModel() {

	// 頂点番号
	enum VertexIndex {
		LeftBottomFront,  //左下前
		LeftTopFront,     //左上前
		RightBottomFront, //右下前
		RightTopFront,    //右上前
		LeftBottomBack,   //左下後
		LeftTopBack,      //左上後
		RightBottomBack,  //右下後
		RightTopBack      //右上後
	};

	// 頂点座標の定義
	Vector4 vertices[8] = {
		{-1.0f,-1.0f,-1.0f,1.0f}, // 左下前
		{-1.0f, 1.0f,-1.0f,1.0f}, // 左上前
		{ 1.0f,-1.0f,-1.0f,1.0f}, // 右下前
		{ 1.0f, 1.0f,-1.0f,1.0f}, // 右上前
		{-1.0f,-1.0f, 1.0f,1.0f}, // 左下後
		{-1.0f, 1.0f, 1.0f,1.0f}, // 左上後
		{ 1.0f,-1.0f, 1.0f,1.0f}, // 右下後
		{ 1.0f, 1.0f, 1.0f,1.0f}, // 右上後
	};

	// 各面の三角形を定義 (三角形を2つずつ使って面を形成)
	uint32_t indices[36] = {
		LeftBottomFront, LeftTopFront, RightBottomFront,  LeftTopFront, RightTopFront, RightBottomFront,      // 前面
		RightBottomBack, RightTopBack, LeftBottomBack,  RightTopBack, LeftTopBack, LeftBottomBack,            // 背面
		LeftBottomBack, LeftTopBack, LeftBottomFront,  LeftTopBack, LeftTopFront, LeftBottomFront,            // 左面
		RightBottomFront, RightTopFront, RightBottomBack,  RightTopFront, RightTopBack, RightBottomBack,      // 右面
		LeftTopFront, LeftTopBack, RightTopFront,  LeftTopBack, RightTopBack, RightTopFront,                  // 上面
		RightBottomFront, RightBottomBack, LeftBottomFront,  RightBottomBack, LeftBottomBack, LeftBottomFront // 底面
	};

	// テクスチャ座標 (各頂点に対応)
	Vector2 texcoords[6] = {
		{0.0f, 1.0f}, // 左下
		{0.0f, 0.0f}, // 左上
		{1.0f, 1.0f}, // 右下
		{0.0f, 0.0f}, // 左上
		{1.0f, 0.0f}, // 右上
		{1.0f, 1.0f}, // 右下
	};

	// 各三角形の頂点データを追加
	for (int i = 0; i < 36; i += 6) {

		VertexData vertex[6];

		for (int j = 0; j < 6; ++j) {

			int index = indices[i + j];

			vertex[j].position = vertices[index];

			vertex[j].texcoord = texcoords[j % 6];

			vertex[j].normal = {
				vertex[j].position.x,
				vertex[j].position.y,
				vertex[j].position.z
			};
		}

		modelData_.vertices.push_back(vertex[0]);
		modelData_.vertices.push_back(vertex[1]);
		modelData_.vertices.push_back(vertex[2]);
		modelData_.vertices.push_back(vertex[3]);
		modelData_.vertices.push_back(vertex[4]);
		modelData_.vertices.push_back(vertex[5]);
	}
}

///=====================================================/// 
/// objファイルの読み込み
///=====================================================///
Model::ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	///-------------------------------------------/// 
	/// ローカル変数
	///-------------------------------------------///

	//構築するModelData
	ModelData modelData;

	//頂点データ
	VertexData triangle[3];

	//位置
	std::vector<Vector4> positions;

	//法線
	std::vector<Vector3> normals;

	//テクスチャ座標
	std::vector<Vector2> texcoords;

	//ファイルから読んだ1行を格納するもの
	std::string line;

	///-------------------------------------------/// 
	/// objファイルからデータを読み込む
	///-------------------------------------------///

	//ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);

	//ファイルが開けたかの確認
	assert(file.is_open());

	while (std::getline(file, line)) {

		//識別子
		std::string identifier;

		//1行
		std::istringstream s(line);

		//先頭の識別子を読む
		s >> identifier;

		if (identifier == "v") {

			/// === 識別子が「v」であれば === ///

			//座標データ
			Vector4 position;

			//ファイルから読み込む
			s >> position.x >> position.y >> position.z;

			//X軸を反転させる
			position.x *= -1.0f;

			position.w = 1.0f;

			//座標データを登録する
			positions.push_back(position);
		} else if (identifier == "vt") {

			/// === 識別子が「vt」だったら === ///

			//テクスチャ座標データ
			Vector2 texCoord;

			//ファイルから読み込む
			s >> texCoord.x >> texCoord.y;

			//
			texCoord.y = 1.0f - texCoord.y;

			//テクスチャ座標データを登録
			texcoords.push_back(texCoord);
		} else if (identifier == "vn") {

			/// === 識別子が「vn」であれば === ///

			//法線データ
			Vector3 normal;

			//ファイルから読み込む
			s >> normal.x >> normal.y >> normal.z;

			//X軸を反転
			normal.x *= -1.0f;

			//法線データを登録
			normals.push_back(normal);
		} else if (identifier == "f") {

			/// === 識別子が「f」だったら === ///

			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {

				//頂点の要素
				std::string vertexDefinition;

				//頂点の要素を読み込む
				s >> vertexDefinition;

				//頂点の要素へのIndexは[位置/UV/法線]で格納されているので、分類してIndexを取得する
				std::istringstream v(vertexDefinition);

				//頂点要素の格納用
				uint32_t elementIndices[3];

				for (int32_t element = 0; element < 3; ++element) {

					//要素
					std::string index;

					//区切りで要素を読み込む
					std::getline(v, index, '/');

					//要素を格納する
					elementIndices[element] = std::stoi(index);
				}

				//格納した要素から値を取り出していく
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texCoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				//三角形の構築
				triangle[faceVertex] = { position,texCoord,normal };
			}

			//頂点データを登録
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);

		} else if (identifier == "mtllib") {

			/// ===  識別子が「mtllib」だったら=== ///

			std::string materialFilename;

			//materialTemplateLibraryファイルの名前を取得する
			s >> materialFilename;

			//マテリアルデータを読み込む
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	return modelData;
}

///=====================================================/// 
/// マテリアルデータの読み込み
///=====================================================///
Model::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	//構築するMaterialData
	MaterialData materialData;

	//ファイルから読んだ1行を格納するもの
	std::string line;

	//ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);

	//開けなかったら止める
	assert(file.is_open());

	while (std::getline(file, line)) {

		//識別子
		std::string identifier;

		//1行
		std::istringstream s(line);

		//先頭から識別子を読み込む
		s >> identifier;

		if (identifier == "map_Kd") {

			/// === 識別子が「map_kd」だったら === ///

			//ファイル名
			std::string textureFilename;

			//ファイル名を読み込む
			s >> textureFilename;

			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}