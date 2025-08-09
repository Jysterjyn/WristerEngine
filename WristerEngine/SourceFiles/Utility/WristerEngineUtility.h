#pragma once
#include <string>
#include <memory>
#include <list>

namespace WristerEngine
{
	// デフォルトリソースパス
	const std::string DEFAULT_RESOURCE_PATH = "Resources/";

	// 個別のリソースパスを生成
	inline const std::string CreateResourcePath(const std::string& directoryPath)
	{
		return DEFAULT_RESOURCE_PATH + directoryPath;
	}

	template<class T> using uPtr = std::unique_ptr<T>;
	template<class T> using uList = std::list<uPtr<T>>;
}

// WristerEngineの略称のエイリアス
namespace WE = WristerEngine;