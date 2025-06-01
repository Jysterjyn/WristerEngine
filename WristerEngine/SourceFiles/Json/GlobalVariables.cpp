#include "GlobalVariables.h"
#include <imgui.h>
#include <ImGuiManager.h>
#include <fstream>
#include <Windows.h>
using namespace WE;

GlobalVariables* GlobalVariables::GetInstance()
{
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update()
{
	// ウィンドウオープン
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar))
	{
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) { return; }

	// 各グループについて
	for (auto itGroup = datas.begin(); itGroup != datas.end(); ++itGroup)
	{
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) { continue; }

		// 各項目について
		for (auto itItem = group.items.begin(); itItem != group.items.end(); ++itItem)
		{
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value))
			{
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}

			// float型の値を保持していれば
			else if (std::holds_alternative<float>(item.value))
			{
				float* ptr = std::get_if<float>(&item.value);
				ImGui::InputFloat(itemName.c_str(), ptr, 0.1f);
			}

			// Vector3型の値を保持していれば
			else if (std::holds_alternative<Vector3>(item.value))
			{
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGuiManager::SliderVector(itemName.c_str(), *ptr, -10.0f, 10.0f);
			}
		}

		// 改行
		ImGui::Text("\n");

		// jsonで保存
		if (ImGui::Button("Save"))
		{
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	// ウィンドウクローズ
	ImGui::EndMenuBar();
	ImGui::End();
}

void WristerEngine::GlobalVariables::SaveFile(const std::string& groupName)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas.find(groupName);
	// 未登録チェック
	assert(itGroup != datas.end());

	json root = json::object();
	// jsonオブジェクト登録
	root[groupName] = json::object();

	// 各項目について
	for (auto itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem)
	{
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value))
		{
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}

		// float型の値を保持していれば
		else if (std::holds_alternative<float>(item.value))
		{
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}

		// Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item.value))
		{
			// float型のjson配列登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({ value.x,value.y,value.z });
		}
	}

	// ディレクトリがなければ作成する
	const std::string kDirectoryPath = CreateResourcePath(DIRECTORY_PATH);
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) { std::filesystem::create_directory(kDirectoryPath); }
	// 書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);
	// ファイルオープン失敗？
	if (ofs.fail())
	{
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();
}

void WristerEngine::GlobalVariables::LoadFile(const std::string& groupName)
{
	const std::string kDirectoryPath = CreateResourcePath(DIRECTORY_PATH);
	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	ifs.open(filePath);
	// ファイルオープン失敗？
	if (ifs.fail())
	{
		std::string message = "Failed open data file for read.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	json::iterator itGroup = root.find(groupName);
	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem)
	{
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer())
		{
			// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}

		// float型の値を保持していれば
		else if (itItem->is_number_float())
		{
			// float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}

		// 要素数3の配列であれば
		else if (itItem->is_array() && itItem->size() == 3)
		{
			// float型のjson配列登録
			Vector3 value = { itItem->at(0),itItem->at(1),itItem->at(2) };
			SetValue(groupName, itemName, value);
		}
	}
}

void WristerEngine::GlobalVariables::LoadFiles()
{
	const std::string kDirectoryPath = CreateResourcePath(DIRECTORY_PATH);
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(kDirectoryPath)) { return; }

	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (auto& entry : dir_it)
	{
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) { continue; }

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

void WristerEngine::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value)
{
	// グループの参照を取得
	Group& group = datas[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void WristerEngine::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value)
{
	// グループの参照を取得
	Group& group = datas[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void WristerEngine::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value)
{
	// グループの参照を取得
	Group& group = datas[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void WristerEngine::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value)
{
	// 項目が未登録なら
	if (!datas[groupName].items.contains(key)) { SetValue(groupName, key, value); }
}

void WristerEngine::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value)
{
	// 項目が未登録なら
	if (!datas[groupName].items.contains(key)) { SetValue(groupName, key, value); }
}

void WristerEngine::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value)
{
	// 項目が未登録なら
	if (!datas[groupName].items.contains(key)) { SetValue(groupName, key, value); }
}

int32_t WristerEngine::GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const
{
	assert(datas.contains(groupName));
	// グループの参照を取得
	const Group& group = datas.at(groupName);
	assert(group.items.contains(key));
	return std::get<int32_t>(group.items.at(key).value);
}

float WristerEngine::GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const
{
	assert(datas.contains(groupName));
	// グループの参照を取得
	const Group& group = datas.at(groupName);
	assert(group.items.contains(key));
	return std::get<float>(group.items.at(key).value);
}

Vector3 WristerEngine::GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const
{
	assert(datas.contains(groupName));
	// グループの参照を取得
	const Group& group = datas.at(groupName);
	assert(group.items.contains(key));
	return std::get<Vector3>(group.items.at(key).value);
}
