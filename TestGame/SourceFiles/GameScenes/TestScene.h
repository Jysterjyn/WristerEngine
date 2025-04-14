#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <FbxLoader.h>
#include <FbxObject3d.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};

class TestScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	WristerEngine::_3D::FbxModel* fModel = nullptr;
	WristerEngine::_3D::FbxObject3d fObject;
	WristerEngine::_3D::Transform fTrans;

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void Draw() { fObject.Draw(); }
};