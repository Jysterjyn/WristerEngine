#pragma once
#include "Matrix4.h"
#include "ParticleGroup.h"

namespace WristerEngine
{
	// パーティクルグループの管理
	class ParticleManager final
	{
	private:
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
			Matrix4 mat;	// 3D変換行列
			Matrix4 matBillboard; // ビルボード行列
		};

		// 定数バッファ
		static ComPtr<ID3D12Resource> constBuff;
		static ConstBufferData* constMap;
		// パーティクルグループの配列
		static std::vector<ParticleGroup> particleGroups;

		ParticleManager() = delete;
		ParticleManager(const ParticleManager& obj) = delete;

	public:
		// 静的初期化
		static void Initialize();
		// 更新
		static void Update();
		// 描画
		static void Draw();
		// パーティクルの削除
		static void Clear();
		// パーティクルグループ追加
		static void AddParticleGroup(const std::string& textureName); // パーティクルグループの追加
		// パーティクルグループ取得
		static ParticleGroup* GetParticleGroup(size_t index) { return &particleGroups[index]; }
	};
}