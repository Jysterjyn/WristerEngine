#pragma once
#include <string>
#include <memory>
#include <list>
#include <set>
#include <vector>

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

	template <class T>
	// 順番を無視して要素が同じかどうかを比較する関数
	bool CompareVectors(const std::vector<T>& a, const std::vector<T>& b) {
		// multisetに変換して比較
		// 要素数も自動的に評価
		return std::multiset<T>(a.begin(), a.end()) == std::multiset<T>(b.begin(), b.end());
	}

	template <class T>
	// 1度しか変更されない値を表すクラス
	class OneChange
	{
		bool isChanged = false;
		T value;

	public:
		/// <summary>
		/// 変更する値を代入する演算子。1度しか変更できない。
		/// </summary>
		/// <param name="newValue">変更する値</param>
		/// <returns>変更されたらtrue</returns>
		bool operator=(const T& newValue)
		{
			if (isChanged) { return false; }
			value = newValue;
			isChanged = true;
			return true;
		}

		operator T() const { return value; }
		bool IsChanged() const { return isChanged; }
	};
}

// WristerEngineの略称のエイリアス
namespace WE = WristerEngine;