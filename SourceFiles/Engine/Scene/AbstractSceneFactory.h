#pragma once
#include "BaseScene.h"
#include <memory>
#include <string>

namespace WristerEngine
{
	// シーンファクトリーの抽象クラス
	class AbstractSceneFactory
	{
	public:
		// 仮想デストラクタ
		virtual ~AbstractSceneFactory() = default;
		// シーン作成
		virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
	};
}