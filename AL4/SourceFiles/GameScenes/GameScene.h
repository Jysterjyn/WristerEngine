#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <skydome.h>
#include <Ground.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override {}
	void Update() override {}
};

class TestScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	WE::Skydome skydome;
	Ground ground;

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};