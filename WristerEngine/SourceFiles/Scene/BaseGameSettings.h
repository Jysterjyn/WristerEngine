#pragma once
#include <Input.h>
#include <unordered_map>
#include <unordered_set>

namespace WristerEngine
{
	// ゲームの基本設定
	class BaseGameSettings
	{
	protected:
		Input* input = Input::GetInstance();
		std::unordered_map<std::string, std::unordered_set<Key>> keyList;

	public:
		virtual ~BaseGameSettings() = default;
		virtual void ChangeResolution() = 0;
		void SetKey(CR<std::string> keyName, Key key){ keyList[keyName].insert(key); }
		CR<std::unordered_set<Key>> GetKeyList(CR<std::string> keyName) const { return keyList.at(keyName); }
	};
}