#include "DebugLine.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "DebugObjectCommon.h"
#include "Camera.h"

#include "string"

DebugLine::DebugLine() {

	//デバッグオブジェクト基底のインスタンスを取得
	debugCommon_ = DebugObjectCommon::GetInstance();

	//カメラ情報を取得
	camera_ = debugCommon_->GetCamera();

	/// === 頂点リソースの生成 === ///

	//頂点リソースを生成
	vertexResource_ = debugCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 2);

	//バッファービューの参照位置を頂点リソースのGPUアドレスで設定
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//バッファビューの全体容量を設定
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 2);

	//バッファビューの1頂点当たりの容量を設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースに頂点データをマッピング
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//頂点座標を設定
	modelData_.vertices = {
		{0.0f,0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f,1.0f}
	};

	modelData_.indexes = {
		0,
		1
	};

	//頂点データに頂点座標を設定する
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	/// ===頂点番号リソースの生成=== ///

	indexResource_ = debugCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 2);

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 2);

	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	std::memcpy(indexData_, modelData_.indexes.data(), sizeof(uint32_t) * modelData_.indexes.size());

	/// === 座標変換行列リソースの生成 === ///

	//座標変換行列リソースの設定
	wvpResource_ = debugCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//座標変換行列リソースに座標変換行列データをマッピング
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));

	/// === マテリアルリソースの生成 === ///

	//マテリアルリソースの生成
	materialResource_ = debugCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//マテリアルリソースにマテリアルデータをマッピング
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//マテリアルの色を設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugLine::Update() {

	//ワールドトランスフォームの更新
	transform_.UpdateMatrix();

	//ワールドビュープロジェクション行列
	Matrix4x4 worldViewProjectionMatrix = transform_.GetWorldMatrix();

	if (camera_) {

		//ビュープロジェクション行列
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewMatrix();

		//ワールド行列にビュープロジェクション行列を乗算
		worldViewProjectionMatrix *= viewProjectionMatrix;
	}

	//座標変換行列データのワールドビュープロジェクション行列を設定
	wvpData_->WVP = worldViewProjectionMatrix;
}

void DebugLine::Draw() {

	//頂点データをGPUに送信
	debugCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//頂点番号データをGPUに送信
	debugCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//マテリアルデータをGPUに送信 : RootParameter0
	debugCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	//座標変換行列データをGPUに送信 : RootParameter1
	debugCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_.Get()->GetGPUVirtualAddress());

	//描画コマンドを送信 : 頂点数2
	debugCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indexes.size()), 1, 0, 0, 0);
}
