#pragma once
#include <BaseScene.h>
#include <Object3d.h>
#include <TimedCall.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	std::unique_ptr<WristerEngine::TimedCall> timedCall;
	// AbstractUIDrawer ����Čp������܂���
	void Initialize() override;
	void Update() override;
	void CreateNewSprite();
};

class TestScene : public WristerEngine::BaseScene
{
	WristerEngine::_3D::Object3d* obj = nullptr;

	// BaseScene ����Čp������܂���
	void Initialize() override;
	void Update() override;
};