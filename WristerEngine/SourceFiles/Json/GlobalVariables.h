#pragma once
#include <variant>
#include <Vector.h>
#include <map>
#include <string>
#include <WristerEngineUtility.h>

namespace WristerEngine
{
	class GlobalVariables final
	{
	public:
		// 項目(usingですっきりさせる:クラス完成後)
		struct Item
		{
			std::variant<int32_t, float, Vector3> value;
		};

		// グループ
		struct Group
		{
			std::map<std::string, Item> items;
		};

	private:
		// 全データ
		std::map<std::string, Group> datas;

		GlobalVariables() = default;
		~GlobalVariables() = default;
		GlobalVariables(const GlobalVariables&) = delete;
		GlobalVariables& operator=(const GlobalVariables&) = delete;

	public:
		static GlobalVariables* GetInstance();

		/// <summary>
		/// グループの作成
		/// </summary>
		/// <param name="groupName">グループ名</param>
		void CreateGroup(const std::string& groupName);

		// 毎フレーム処理
		void Update();

		// 値をセット(int)
		// 後で関数テンプレートでまとめる
		void SetValue(const std::string& groupName, const std::string& key, int32_t value);
		// 値をセット(float)
		void SetValue(const std::string& groupName, const std::string& key, float value);
		// 値をセット(Vector3)
		void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
	};
}