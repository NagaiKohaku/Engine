#pragma once

class SceneManager;

class BaseScene {

public:

	virtual ~BaseScene() = default;

	virtual void Initialize();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw();

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	SceneManager* sceneManager_ = nullptr;

private:

};