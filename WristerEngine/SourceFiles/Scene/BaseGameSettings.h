#pragma once
#include <Input.h>
#include <unordered_map>

namespace WristerEngine
{
	// ゲームの基本設定
	class BaseGameSettings
	{
	protected:
		Input* input = Input::GetInstance();
		std::unordered_map<std::string, Key> keyList;

	public:
		virtual ~BaseGameSettings() = default;
		void SetKey(const std::string& keyName, Key key) { keyList[keyName] = key; }
		Key GetKey(const std::string& keyName) const { return keyList.at(keyName); }
	};
}