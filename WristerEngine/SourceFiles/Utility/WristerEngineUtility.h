#pragma once
#include <string>

namespace WristerEngine
{
	// デフォルトリソースパス
	const std::string DEFAULT_RESOURCE_PATH = "Resources/";

	// 個別のリソースパスを生成
	inline const std::string CreateResourcePath(const std::string& directoryPath)
	{
		return DEFAULT_RESOURCE_PATH + directoryPath;
	}
}

// WristerEngineの略称のエイリアス
namespace WE = WristerEngine;