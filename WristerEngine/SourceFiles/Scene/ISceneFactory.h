#pragma once
#include "BaseScene.h"
#include <memory>
#include <string>

namespace WristerEngine
{
	// シーンファクトリーの抽象クラス
	class ISceneFactory
	{
	public:
		// 仮想デストラクタ
		virtual ~ISceneFactory() = default;
		// シーン作成
		virtual uPtr<BaseScene> CreateScene(CR<std::string> sceneName) = 0;
	};
}