#pragma once
#include <BaseScene.h>
#include <Object3d.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ����Čp������܂���
	void Initialize() override;
};

class TestScene : public WristerEngine::BaseScene
{
	WristerEngine::_3D::Object3d* obj = nullptr;

	// BaseScene ����Čp������܂���
	void Initialize() override;
	void Update() override;
};