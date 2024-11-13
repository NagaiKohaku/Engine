#include "ParticleEmitter.h"
#include "ParticleManager.h"

//1フレームで進む秒数
const float ParticleEmitter::kDeltaTime = 1.0f / 60.0f;

///=====================================================/// 
/// コンストラクタ
///=====================================================///
ParticleEmitter::ParticleEmitter()
	:transform_({
		{ 1.0f,1.0f,1.0f },
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f }
		}
	),
	count_(3),
	frequency_(0.3f),
	frequencyTime_(0.0f) {
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void ParticleEmitter::Update() {

	//タイマーを進ませる
	frequencyTime_ += kDeltaTime;

	//パーティクルグループ
	std::unordered_map<std::string, ParticleManager::ParticleGroup> particleGroups;

	//パーティクルグループの情報を取得
	particleGroups = ParticleManager::GetInstance()->GetParticleGroup();

	//タイマーが発生間隔を越えていたら
	if (frequency_ <= frequencyTime_) {

		for (std::unordered_map<std::string, ParticleManager::ParticleGroup>::iterator it = particleGroups.begin();
			it != particleGroups.end();it++) {

			//パーティクルを生成
			ParticleManager::GetInstance()->Emit(it->first, transform_.translate, count_);
		}

		//タイマーをリセット
		frequencyTime_ -= frequency_;
	}

}

///=====================================================/// 
/// パーティクルを生成
///=====================================================///
void ParticleEmitter::Emit() {

	for (std::unordered_map<std::string, ParticleManager::ParticleGroup>::iterator it = ParticleManager::GetInstance()->GetParticleGroup().begin();
		it != ParticleManager::GetInstance()->GetParticleGroup().end();) {

		ParticleManager::GetInstance()->Emit(it->first, transform_.translate, count_);
	}
}