#pragma once
#include "Matrix4.h"
#include "ParticleGroup.h"
#include "ModelManager.h"

namespace WristerEngine
{
	enum class ParticleType
	{
		Light,	// 光パーティクル
		Dark	// 闇パーティクル
	};

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
		ComPtr<ID3D12Resource> constBuff;
		ConstBufferData* constMap = nullptr;
		// パーティクルグループの配列
		std::unordered_map<ParticleType, std::vector<ParticleGroup>> particleGroups;
		_3D::ModelManager* modelManager = nullptr;

		ParticleManager() = default;
		~ParticleManager() = default;
		ParticleManager(const ParticleManager&) = delete;
		ParticleManager& operator=(const ParticleManager&) = delete;

	public:
		// インスタンス取得
		static ParticleManager* GetInstance();
		// 静的初期化
		void Initialize();
		// 更新
		void Update();
		// 描画
		void Draw();
		// パーティクルの削除
		void Clear();
		// パーティクルグループ追加
		void AddParticleGroup(const std::string& textureName, ParticleType particleType = ParticleType::Light);
		// パーティクルグループ取得
		ParticleGroup* GetParticleGroup(size_t index, ParticleType particleType = ParticleType::Light) { return &particleGroups[particleType][index]; }
	};
}