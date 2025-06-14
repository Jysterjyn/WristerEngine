#pragma once
#include "GameObject.h"
#include <list>

namespace WristerEngine::_3D
{
	// ステージ上にあるゲーム表示物管理クラス
	class BaseStage
	{
	private:
		// ステージ上にあるゲーム表示物
		std::list<GameObject> stageObjects;
		// Jsonから読み込んだデータ
		//ObjectData objData;

	protected:
		// 固有初期化
		virtual void VirtualInitialize() {};
		// 固有更新
		virtual void VirtualUpdate() {};

		// 初期化
		void Initialize();
		// 更新
		void Update();
	};
}