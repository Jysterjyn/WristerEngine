#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	// ワールドトランスフォーム
	class Transform
	{
	private:
		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
			Matrix4 world; // ワールド行列
		};

	public:
		Matrix4 matWorld;
		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		Vector3 scale = { 1,1,1 }, rotation, translation;
		Transform* parent = nullptr;
		bool isUpdated = false;

		// 初期化
		void Initialize();
		// 更新
		void Update();
		// 描画
		void Draw();
		// getter
		Vector3 GetWorldPosition() const { return { matWorld.m[3][0],matWorld.m[3][1],matWorld.m[3][2] }; }
		Vector3 GetScale() const;
	};
}