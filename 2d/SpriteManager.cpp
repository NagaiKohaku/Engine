#include "SpriteManager.h"
#include "SpriteCommon.h"

SpriteManager* SpriteManager::GetInstance() {
	static SpriteManager instance;
	return &instance;
}

void SpriteManager::Initialize() {

	spriteCommon_ = SpriteCommon::GetInstance();
}

void SpriteManager::LoadSprite(const std::string& spriteName,const std::string& spriteFileName) {

	if (sprites_.contains(spriteName)) {

		return;
	}

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();

	sprite->Initialize("Resource/Sprite/" + spriteFileName + ".png");

	sprites_.insert(std::make_pair(spriteName, std::move(sprite)));
}

Sprite* SpriteManager::FindSprite(const std::string& spriteName) {

	if (sprites_.contains(spriteName)) {

		return sprites_.at(spriteName).get();
	}

	return nullptr;
}
