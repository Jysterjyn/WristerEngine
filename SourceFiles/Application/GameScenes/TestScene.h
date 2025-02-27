#pragma once
#include <BaseScene.h>
#include <Object3d.h>
#include <TimedCall.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	std::unique_ptr<WristerEngine::TimedCall> timedCall;
	//WristerEngine::TimedCall* timedCall = nullptr;
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void CreateNewSprite();
};

class TestScene : public WristerEngine::BaseScene
{
	WristerEngine::_3D::Object3d* obj = nullptr;

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};