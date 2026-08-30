#pragma once
#include "Framework.h"
#include <PostEffect.h>
#include <thread>

class TestClass : public WE::ListObject
{
	int objNum = 0;
	bool isDelete = false;
	WE::Input* input = WE::Input::GetInstance();

	void Initialize() override {}
	void Update() override;
	bool Remove() override { return isDelete; };

public:
	TestClass(int num) { objNum = num; }
};

// このゲーム固有の処理クラス
class MyGame : public WE::Framework
{
	WE::Test testList;
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