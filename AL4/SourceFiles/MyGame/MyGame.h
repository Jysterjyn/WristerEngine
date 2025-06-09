#pragma once
#include "Framework.h"
#include "PostEffect.h"
#include "ModelManager.h"

// このゲーム固有の処理クラス
class MyGame : public WristerEngine::Framework
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();

	// 初期化(オーバーライド)
	void Initialize() override;
	// 更新(オーバーライド)
	void Update() override;
	// 描画(オーバーライド)
	void Draw() override;
	// 終了処理(オーバーライド)
	void Finalize() override;
	bool IsEndRequest() override;
};