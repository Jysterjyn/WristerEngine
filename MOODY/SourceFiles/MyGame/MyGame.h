#pragma once
#include "Framework.h"
#include <PostEffect.h>
#include <thread>

// このゲーム固有の処理クラス
class MyGame : public WE::Framework
{
	WE::Audio* bgm = nullptr;
	REFTIME bgmTime = 0;

	void ImGuiTexts();

	// 初期化(オーバーライド)
	void Initialize();
	// 更新(オーバーライド)
	void Update();
	// 描画(オーバーライド)
	void Draw();
	// 終了処理(オーバーライド)
	void Finalize() {}
	bool IsEndRequest() override;

public:
	MyGame(WE::CR<std::wstring> windowName_) : Framework(windowName_) {}
	~MyGame() = default;
};