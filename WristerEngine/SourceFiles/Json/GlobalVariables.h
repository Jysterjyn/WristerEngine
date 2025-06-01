#pragma once
#include <variant>
#include <Vector.h>
#include <map>
#include <string>
#include <WristerEngineUtility.h>
#include <json.hpp>

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
		using json = nlohmann::json;
		// グローバル変数の保存先ファイルパス
		const std::string DIRECTORY_PATH = "GlobalVariables/";

		// 全データ
		std::map<std::string, Group> datas;

		GlobalVariables() = default;
		~GlobalVariables() = default;
		GlobalVariables(const GlobalVariables&) = delete;
		GlobalVariables& operator=(const GlobalVariables&) = delete;

	public:
		static GlobalVariables* GetInstance();

		// 毎フレーム処理
		void Update();

		/// <summary>
		/// ファイルに書き出し
		/// </summary>
		/// <param name="groupName">グループ</param>
		void SaveFile(const std::string& groupName);

		/// <summary>
		/// ファイルから読み込む
		/// </summary>
		/// <param name="groupName">グループ</param>
		void LoadFile(const std::string& groupName);

		// ディレクトリの全ファイル読み込み
		void LoadFiles();

		// 後で関数テンプレートでまとめる
		// 値をセット(int)
		void SetValue(const std::string& groupName, const std::string& key, int32_t value);
		// 値をセット(float)
		void SetValue(const std::string& groupName, const std::string& key, float value);
		// 値をセット(Vector3)
		void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
		// 項目の追加(int)
		void AddItem(const std::string& groupName, const std::string& key, int32_t value);
		// 項目の追加(float)
		void AddItem(const std::string& groupName, const std::string& key, float value);
		// 項目の追加(Vector3)
		void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);
		
		// 値の取得(後で関数テンプレートでまとめる)
		int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
		float GetFloatValue(const std::string& groupName, const std::string& key) const;
		Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;
	};
}