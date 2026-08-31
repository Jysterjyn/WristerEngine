#pragma once
#include <string>
#include <memory>
#include <list>
#include <set>
#include <vector>
#include <functional>

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
	template<class T> using CR = const T&;

	class ListObject
	{
	public:
		virtual ~ListObject() = default;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() {}
		virtual bool Remove() const { return false; }
	};

	class uList2 : public std::list<uPtr<ListObject>>
	{
		//std::list<uPtr<ListObject>> list;

	public:
		ListObject* Add(uPtr<ListObject> obj)
		{
			obj->Initialize();
			push_back(std::move(obj));
			return back().get();
		}

		void Update()
		{
			remove_if([&](CR<uPtr<ListObject>> item) { return item->Remove(); });
			for (auto& item : *this) { item->Update(); }
		}

		void Draw() { for (auto& item : *this) { item->Draw(); } }
	};

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

		const T Get() const { return value; }
		bool IsChanged() const { return isChanged; }
	};

	template <class T>
	uint32_t ChangeVal(T value)
	{
		return static_cast<uint32_t>(value);
	}
}

// WristerEngineの略称のエイリアス
namespace WE = WristerEngine;