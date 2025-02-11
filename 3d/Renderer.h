#pragma once

#include "LayerType.h"

#include "functional"
#include "map"
#include "vector"

class Renderer {

public:

	static Renderer* GetInstance();

	void Initialize();

	void Draw();

	void AddDraw(LayerType type,std::function<void()> func);

private:

	void ClearQueue();

private:

	std::map<LayerType, std::vector<std::function<void()>>> drawQueue_;

};