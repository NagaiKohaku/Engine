#include "Sprite.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "TextureManager.h"

#include "MakeMatrixMath.h"

#include "imgui.h"

///=====================================================/// 
/// 初期化処理
///=====================================================///
void Sprite::Initialize(const std::string& fileName) {

	//スプライト基底のインスタンスを取得
	spriteCommon_ = SpriteCommon::GetInstance();

	//テクスチャパスの設定
	texturePath_ = "resources/" + fileName;

	//頂点リソースを作成
	vertexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);

	//頂点インデックスリソースを作成
	IndexResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	//マテリアルリソースを作成
	materialResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//座標変換行列リソース
	WVPResource_ = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//リソースの先頭のアドレスを取得する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	indexBufferView_.BufferLocation = IndexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//書き込むためのアドレスを取得する
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	IndexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();

	//座標変換行列データの設定
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();

	//テクスチャをロードしてテクスチャ番号を取得
	TextureManager::GetInstance()->LoadTexture(texturePath_);

	//サイズをテクスチャの解像度に合わせる
	AdjustTextureSize();
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void Sprite::Update() {

	//アンカーポイントから四点を計算
	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	//X軸の反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	//Y軸の反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	//テクスチャのメタデータを取得
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(texturePath_);

	//テクスチャの四点を計算
	float texLeft = textureLeftTop_.x / metadata.width;
	float texRight = (textureLeftTop_.x + textureSize_.x) / metadata.width;
	float texTop = textureLeftTop_.y / metadata.height;
	float texBottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

	//頂点リソースのデータを書き込む
	//左下
	vertexData_[0].position = { left,bottom,0.0f,1.0f };
	vertexData_[0].texcoord = { texLeft,texBottom };
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
	//左上
	vertexData_[1].position = { left,top,0.0f,1.0f };
	vertexData_[1].texcoord = { texLeft,texTop };
	vertexData_[1].normal = { 0.0f,0.0f,-1.0f };
	//右下
	vertexData_[2].position = { right,bottom,0.0f,1.0f };
	vertexData_[2].texcoord = { texRight,texBottom };
	vertexData_[2].normal = { 0.0f,0.0f,-1.0f };
	//右上
	vertexData_[3].position = { right,top,0.0f,1.0f };
	vertexData_[3].texcoord = { texRight,texTop };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	//頂点インデックスのデータを書き込む
	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

	//トランスフォームを構築
	Transform transform{
		{size_.x,size_.y,1.0f},
		{0.0f,0.0f,rotation_},
		{position_.x,position_.y,0.0f}
	};

	//ワールド行列を生成
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	//ビュー行列を単位行列で生成
	Matrix4x4 viewMatrix = MakeIdentity4x4();

	//プロジェクション行列を生成
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 100.0f);

	//WVPデータを設定
	WVPData_->WVP = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	WVPData_->World = worldMatrix;
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Sprite::Draw() {

	//頂点データの設定
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//頂点番号データの設定
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//マテリアルデータの設定
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//座標変換行列データの設定
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_->GetGPUVirtualAddress());

	//テクスチャの設定
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(texturePath_));

	//描画命令
	spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void Sprite::DisplayImGui() {

	ImGui::DragFloat2("Position", &position_.x, 1.0f);
	ImGui::SliderAngle("Rotation", &rotation_);
	ImGui::DragFloat2("Size", &size_.x, 0.1f);
	ImGui::ColorEdit4("Color", &materialData_->color.x);
	ImGui::DragFloat2("AnchorPoint", &anchorPoint_.x, 0.01f,0.0f,1.0f);
	ImGui::Checkbox("IsFlipX", &isFlipX_);
	ImGui::Checkbox("IsFlipY", &isFlipY_);
	ImGui::DragFloat2("TexLeftTop", &textureLeftTop_.x, 0.1f);
	ImGui::DragFloat2("TexSize", &textureSize_.x, 0.1f);

}

///=====================================================/// 
/// スプライトのサイズをテクスチャのサイズに合わせる
///=====================================================///
void Sprite::AdjustTextureSize() {

	//テクスチャのメタデータを取得
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(texturePath_);

	//テクスチャのサイズを取得
	textureSize_.x = static_cast<float>(metadata.width);
	textureSize_.y = static_cast<float>(metadata.height);

	//スプライトのサイズをテクスチャのサイズに設定
	size_ = textureSize_;
}

///=====================================================/// 
/// テクスチャの変更
///=====================================================///
void Sprite::ChangeTexture(const std::string& filePath) {

	//ファイルパスからテクスチャを読み込む
	TextureManager::GetInstance()->LoadTexture(filePath);

	//メタデータを取得
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(filePath);

	//テクスチャパスを現在のものに変更
	texturePath_ = filePath;

	//スプライトのサイズをテクスチャのサイズに合わせる
	AdjustTextureSize();
}