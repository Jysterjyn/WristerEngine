#pragma once
#include "Framework.h"
#include "PostEffect.h"
#include <ModelManager.h>
#include <Skydome.h>

// このゲーム固有の処理クラス
class MyGame : public WristerEngine::Framework
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	std::unique_ptr<WristerEngine::Skydome> skydome;

	// 初期化(オーバーライド)
	void Initialize();
	// 更新(オーバーライド)
	void Update();
	// 描画(オーバーライド)
	void Draw();
	// 終了処理(オーバーライド)
	void Finalize();
};