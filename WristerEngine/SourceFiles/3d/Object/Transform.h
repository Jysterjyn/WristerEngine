#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	class Object3d;

	// ワールドトランスフォーム
	class Transform
	{
	private:
		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
			Matrix4 world; // ワールド行列
		};

		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		bool isObject3d = false; // Object3Dのメンバ変数か
		bool isUpdated = false;
		friend Object3d;

	public:
		Matrix4 matWorld;
		Vector3 scale = { 1,1,1 }, rotation, translation;
		Transform* parent = nullptr;

		// 初期化
		void Initialize();
		// 更新
		void Update();
		// 描画
		void Draw();
		// getter
		Vector3 GetWorldPosition() const { return matWorld.GetVector(3); }
		Vector3 GetScale() const;
	};
}

namespace WristerEngine::_2D
{
	// ワールドトランスフォーム
	class Transform
	{
	private:
		bool isUpdated = false;

	public:
		Matrix4 matWorld;
		Vector2 size = { 1,1 }, position;
		float rotation = 0;
		Transform* parent = nullptr;

		// 初期化
		void Initialize();
		// 更新
		void Update();
		// 描画
		void Draw();
		// getter
		Vector2 GetWorldPosition() const { return matWorld.GetVector(3); }
		Vector2 GetScale() const;
	};
}