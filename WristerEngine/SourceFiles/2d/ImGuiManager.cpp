#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include "D3D12Common.h"
using namespace ImGui;
using namespace WristerEngine;
using namespace _2D;

void ImGuiManager::Initialize()
{
	// インスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ImGuiのコンテキストを生成
	CreateContext();
	// ImGuiのスタイルを設定
	StyleColorsDark();

	ImGui_ImplWin32_Init(WindowsAPI::GetInstance()->GetHwnd());

	SRVHandle srvHandle = dxCommon->GetNextSRVHandle();

	ImGui_ImplDX12_Init(dxCommon->GetDevice(),
		static_cast<int>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, dxCommon->GetSRV(),
		srvHandle.cpu, srvHandle.gpu);

	dxCommon->IncrementSRVIndex();

	ImGuiIO& io = GetIO();
	// 標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
}

void ImGuiManager::End() { Render(); }

void ImGuiManager::Draw()
{
	ImGui_ImplDX12_RenderDrawData(GetDrawData(), DirectXCommon::GetInstance()->GetCommandList());
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
}

void ImGuiManager::PrintVector(std::string str, const Vector2& vec)
{
	str += " = (%.3f, %.3f)";
	Text(str.c_str(), vec.x, vec.y);
}

void ImGuiManager::PrintVector(std::string str, const Vector3& vec)
{
	str += " = (%.3f, %.3f, %.3f)";
	Text(str.c_str(), vec.x, vec.y, vec.z);
}

void ImGuiManager::PrintQuaternion(std::string str, const Quaternion& q)
{
	str += " = (%.3f, %.3f, %.3f, %.3f)";
	Text(str.c_str(), q.w, q.x, q.y, q.z);
}

void ImGuiManager::SliderVector(std::string str, Vector2& vec, float minVal, float maxVal)
{
	float num[2] = { vec.x,vec.y };
	SliderFloat2(str.c_str(), num, minVal, maxVal);
	vec = { num[0],num[1] };
}

void ImGuiManager::SliderVector(std::string str, Vector3& vec, float minVal, float maxVal)
{
	float num[3] = { vec.x,vec.y,vec.z };
	SliderFloat3(str.c_str(), num, minVal, maxVal);
	vec = { num[0],num[1],num[2] };
}

void ImGuiManager::DragVector(std::string str, Vector2& vec, float spd)
{
	float num[2] = { vec.x,vec.y };
	DragFloat2(str.c_str(), num, spd);
	vec = { num[0],num[1] };
}

void ImGuiManager::DragVector(std::string str, Vector3& vec, float spd)
{
	float num[3] = { vec.x,vec.y,vec.z };
	DragFloat3(str.c_str(), num, spd);
	vec = { num[0],num[1],num[2] };
}

void ImGuiManager::InputVector(std::string str, Vector2& vec)
{
	float num[2] = { vec.x,vec.y };
	InputFloat2(str.c_str(), num);
	vec = { num[0],num[1] };
}

void ImGuiManager::InputVector(std::string str, Vector3& vec)
{
	float num[3] = { vec.x,vec.y,vec.z };
	InputFloat3(str.c_str(), num);
	vec = { num[0],num[1],num[2] };
}

void ImGuiManager::ColorEdit(std::string str, ColorRGB& color)
{
	float c[3] = { color.r,color.g,color.b };
	str += "Color";
	ColorEdit3(str.c_str(), c);
	color = { c[0],c[1],c[2] };
}

void ImGuiManager::ColorEdit(std::string str, ColorRGBA& color)
{
	float c[4] = { color.r,color.g,color.b,color.a };
	str += "Color";
	ColorEdit4(str.c_str(), c);
	color = { c[0],c[1],c[2],c[3] };
}