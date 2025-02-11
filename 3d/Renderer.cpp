#include "Renderer.h"

Renderer* Renderer::GetInstance() {
	static Renderer instance;
	return &instance;
}

void Renderer::Initialize() {

	ClearQueue();
}

void Renderer::Draw() {

	for (const auto& [type, func] : drawQueue_) {

		for (int i = 0; i < drawQueue_[type].size(); i++) {

			func[i]();
		}
	}
}

void Renderer::AddDraw(LayerType type, std::function<void()> func) {

	drawQueue_[type].push_back(func);
}

void Renderer::ClearQueue() {

	for (auto& [type, func] : drawQueue_) {
		func.clear();
	}
}