#pragma once
#include "Framework.h"

// このゲーム固有の処理クラス
class MyGame : public WE::Framework
{
	// 初期化(オーバーライド)
	void Initialize();
	// 更新(オーバーライド)
	void Update();
	// 描画(オーバーライド)
	void Draw();
	// 終了処理(オーバーライド)
	void Finalize();
};