#pragma once
#include <variant>
#include <Vector.h>
#include <map>
#include <string>
#include <WristerEngineUtility.h>
#pragma warning(push)
#pragma warning(disable:26819)
#include <json.hpp>
#pragma warning(pop)

namespace WristerEngine
{
	// グローバル変数
	class GlobalVariables final
	{
	public:
		// 項目
		using Item = std::variant<int32_t, float, Vector3, bool>;

		// グループ
		using Group = std::map<std::string, Item>;

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

		/// <summary>
		/// ファイルから読み込む
		/// </summary>
		/// <param name="groupName">グループ</param>
		void LoadFile(const std::string& groupName);

	public:
		static GlobalVariables* GetInstance();

		// 毎フレーム処理
		void Update();

		/// <summary>
		/// ファイルに書き出し
		/// </summary>
		/// <param name="groupName">グループ</param>
		void SaveFile(const std::string& groupName);

		// ディレクトリの全ファイル読み込み
		void LoadFiles();

		// 値をセット
		template<class T>
		void SetValue(const std::string& groupName, const std::string& key, const T& value)
		{
			// グループの参照を取得
			Group& group = datas[groupName];
			// 新しい項目のデータを設定
			Item newItem{};
			newItem = value;
			// 設定した項目をstd::mapに追加
			group[key] = newItem;
		}

		// 項目の追加
		template<class T>
		void AddItem(const std::string& groupName, const std::string& key, const T& value)
		{
			// 項目が未登録なら
			if (!datas[groupName].contains(key)) { SetValue(groupName, key, value); }
		}

		// 値の取得
		template<class T>
		T GetValue(const std::string& groupName, const std::string& key) const
		{
			assert(datas.contains(groupName));
			// グループの参照を取得
			const Group& group = datas.at(groupName);
			assert(group.contains(key));
			return std::get<T>(group.at(key));
		}
	};
}