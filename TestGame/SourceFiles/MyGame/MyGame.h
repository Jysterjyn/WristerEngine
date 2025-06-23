#pragma once
#include "Framework.h"
#include "PostEffect.h"
#include "ModelManager.h"
#include <PrimitiveDrawer.h>
#include "ParticleManager.h"

// このゲーム固有の処理クラス
class MyGame : public WE::Framework
{
	WE::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	WE::_3D::PrimitiveDrawer* pDrawer = WE::_3D::PrimitiveDrawer::GetInstance();
	WE::ParticleManager* particleManager = WE::ParticleManager::GetInstance();

	// 初期化(オーバーライド)
	void Initialize();
	// 更新(オーバーライド)
	void Update();
	// 描画(オーバーライド)
	void Draw();
	// 終了処理(オーバーライド)
	void Finalize();
};