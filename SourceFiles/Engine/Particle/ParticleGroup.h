#pragma once
#include "DiffuseParticle.h"
#include "DirectionalParticle.h"
#include "TrackParticle.h"
#include "Sprite.h"
#include <string>
#include <wrl.h>
#include <d3dx12.h>

namespace WristerEngine
{
	// パーティクルグループ
// このインスタンス1つにつき1枚のテクスチャが使える
	class ParticleGroup
	{
	private:
		// 頂点データ構造体
		struct VertexPos
		{
			Vector3 pos; // xyz座標
			float scale; // スケール
		};

		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		static const int PARTICLE_MAX = 384; // パーティクル最大数
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertBuff;
		VertexPos* vertMap;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView;
		// テクスチャインデックス
		_2D::TextureData* texture = nullptr;
		// 拡散するパーティクル
		DiffuseParticle diffuseParticle;
		// 始点から終点へ向かうパーティクル
		DirectionalParticle directionalParticle;
		TrackParticle trackParticle;

		// 頂点バッファ生成
		void CreateVertexBuffer();
		// 全てのパーティクルの合計数
		size_t AllParticleNum() { return diffuseParticle.GetParticles().size() + directionalParticle.GetParticles().size() + trackParticle.GetParticles().size(); }
		// パーティクルが最大値に達してるか
		bool IsParticleMax() { return AllParticleNum() >= PARTICLE_MAX; }

	public:
		// 初期化
		void Initialize(const std::string& textureName);
		// 更新
		void Update();
		// 描画
		void Draw();
		// パーティクルの追加
		void Add(const DiffuseParticle::AddProp& particleProp);
		void Add(const DirectionalParticle::AddProp& particleProp);
		void Add(const TrackParticle::AddProp& particleProp);
		// パーティクルの削除
		void Clear();
	};
}