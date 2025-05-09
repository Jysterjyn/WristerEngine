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
	texture= TextureData::Load("Particles/" + textureName);
	CreateVertexBuffer();
}

void ParticleGroup::Update()
{
	diffuseParticle.Update();
	directionalParticle.Update();

	// 定数バッファへデータ転送
	std::list<DiffuseParticle::Particle> diffuse = diffuseParticle.GetParticles();
	std::list<DirectionalParticle::Particle> directional = directionalParticle.GetParticles();
	int i = 0;

	for (auto& dif : diffuse)
	{
		vertMap[i].pos = dif.position;
		if (dif.parent) { vertMap[i].pos += dif.parent->GetWorldPosition(); }
		vertMap[i].vel = dif.velocity;
		vertMap[i].acc = dif.accel;
		vertMap[i].scales = { dif.s_scale,dif.e_scale };
		vertMap[i].passTime.x = (float)dif.frame.GetInterval();
		vertMap[i].passTime.y = (float)dif.frame.GetTime();
		vertMap[i++].type = 0;
	}
	for (auto& dir : directional)
	{
		vertMap[i].pos = dir.position;
		vertMap[i].scales.x = dir.scale;
		vertMap[i++].type = 1;
	}
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

void ParticleGroup::Add(const DiffuseParticle::AddProp& particleProp)
{
	if (IsParticleMax()) { return; }

	// 最大パーティクル量を超えるのを阻止
	UINT32 nextParticleNum = (UINT32)diffuseParticle.GetParticles().size() + particleProp.addNum;
	if (nextParticleNum > PARTICLE_MAX)
	{
		DiffuseParticle::AddProp p = particleProp;
		p.addNum -= nextParticleNum - PARTICLE_MAX;
		diffuseParticle.Add(p);
		return;
	}

	diffuseParticle.Add(particleProp);
}

void ParticleGroup::Add(const DirectionalParticle::AddProp& particleProp)
{
	if (IsParticleMax()) { return; }
	directionalParticle.Add(particleProp);
}

void ParticleGroup::Clear()
{
	diffuseParticle.Clear();
	directionalParticle.Clear();
}
