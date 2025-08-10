#include "ParticleGroup.h"
#include "D3D12Common.h"
using namespace WE;
using namespace _2D;

void ParticleGroup::CreateVertexBuffer()
{
	CreateBuffer(&vertBuff, &vertMap, PARTICLE_MAX * sizeof(VertexPos));

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = PARTICLE_MAX * sizeof(VertexPos);
	vbView.StrideInBytes = sizeof(VertexPos);
}

void ParticleGroup::Initialize(const std::string& textureName)
{
	// テクスチャ読み込み
	texture = spMan->LoadTexture("Particles/" + textureName);
	CreateVertexBuffer();
}

void ParticleGroup::Update()
{
	for (auto& p : particles) { p->Update(); }
	particles.remove_if([](std::unique_ptr<Particle>& p) { return p->IsDestroy(); });

	// 定数バッファへデータ転送
	size_t i = 0;
	for (auto& p : particles)
	{
		vertMap[i].pos = p->GetPosition();
		vertMap[i].scale = p->GetScale();
		vertMap[i].color = p->GetColor();
		i++;
	}
	for (; i < PARTICLE_MAX; i++) { vertMap[i].scale = 0; }
}

void ParticleGroup::Draw()
{
	if (AllParticleNum() == 0) { return; }
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(0, texture->srvHandle.gpu);
	// 描画コマンド
	cmdList->DrawInstanced((UINT)AllParticleNum(), 1, 0, 0);
}

void ParticleGroup::Add(const BaseParticleProp& particleProp, size_t addNum)
{
	for (size_t i = 0; i < addNum; i++)
	{
		if (IsParticleMax()) { return; }
		std::unique_ptr<Particle> newParticle;
		switch (particleProp.kind)
		{
		case ParticleKind::Diffuse:
			newParticle = std::make_unique<DiffuseParticle>();
			break;
		case ParticleKind::Directional:
			newParticle = std::make_unique<DirectionalParticle>();
			break;
		}
		newParticle->Initialize(particleProp);
		particles.push_back(std::move(newParticle));
	}
}

void ParticleGroup::Clear()
{
	particles.clear();
}
