#include "GlobalVariables.h"
#include <imgui.h>
#include <ImGuiManager.h>
using namespace WE;

GlobalVariables* GlobalVariables::GetInstance()
{
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName)
{
	// 指定名のオブジェクトがなければ追加する
	datas[groupName];
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

		ImGui::EndMenu();
	}

	// ウィンドウクローズ
	ImGui::EndMenuBar();
	ImGui::End();
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
