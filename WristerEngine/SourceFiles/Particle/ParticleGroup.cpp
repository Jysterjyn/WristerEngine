#include "ParticleGroup.h"
#include "D3D12Common.h"
using namespace WristerEngine;
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
	texture = TextureData::Load("Particles/" + textureName);
	CreateVertexBuffer();
}

void ParticleGroup::Update()
{
	for (auto& p : particles) { p->Update(); }
	particles.remove_if([](std::unique_ptr<Particle>& p) { return p->isDestroy; });

	// 定数バッファへデータ転送
	size_t i = 0;
	for (auto& p : particles)
	{
		vertMap[i].pos = p->position;
		vertMap[i].scale = p->scale;
		vertMap[i].color = p->color;
		i++;
	}
	for (; i < PARTICLE_MAX; i++) { vertMap[i].scale = 0; }
}

void ParticleGroup::Draw()
{
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(0, texture->srvHandle.gpu);
	// 描画コマンド
	cmdList->DrawInstanced((UINT)AllParticleNum(), 1, 0, 0);
}

void ParticleGroup::Add(const BaseParticleProp& particleProp)
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

void ParticleGroup::Clear()
{
	particles.clear();
}
