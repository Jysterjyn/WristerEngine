#pragma once
#include "Constant.h"
#include "Sprite.h"
#include <map>

namespace WristerEngine::_2D
{
	// UI描画の抽象クラス
	// 各シーンごとにUIDrawerクラスを定義する
	class AbstractUIDrawer
	{
	protected:
		Constant* constant = Constant::GetInstance();
		std::map<std::string, std::unique_ptr<Sprite>> sprites;

	public:
		// 仮想デストラクタ
		virtual ~AbstractUIDrawer() = default;
		// 初期化(純粋仮想関数)
		virtual void Initialize() = 0;
		// 更新(仮想関数)
		virtual void Update() { for (auto& s : sprites) { s.second->Update(); } }
		// 描画(仮想関数)
		virtual void Draw() { for (auto& s : sprites) { s.second->Draw(); } }
	};
}