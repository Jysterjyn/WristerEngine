#include "DiffuseParticle.h"
#include "Random.h"
#include <cassert>
using namespace WE;

DiffuseParticle::Prop::Prop()
{
	kind = ParticleKind::Diffuse;
	range.position = { 1,1,1 };
	range.velocity = { 0.1f,0.1f,0.1f };
	range.acceleration = { 0.001f,0.001f,0.001f };
}

void DiffuseParticle::Initialize(const BaseParticleProp& prop)
{
	const Prop* addProp = static_cast<const Prop*>(&prop);
	assert(addProp);
	position = RandomVector(addProp->range.position) + addProp->offset.position;
	velocity = RandomVector(addProp->range.velocity) + addProp->offset.velocity;
	accel = RandomVector(addProp->range.acceleration) + addProp->offset.acceleration;
	frame = addProp->lifeTime;
	scales = addProp->scales;
	parent = addProp->parent;
}

void DiffuseParticle::Update()
{
	position += velocity;
	velocity += accel;
	scale = Lerp(scales.x, scales.y, frame.GetTimeRate());
	if (frame.Update()) { Destroy(); }
	color = ColorRGBA::White();
}

const Vector3 WristerEngine::DiffuseParticle::GetPosition() const
{
	Vector3 pos = position;
	if (parent) { pos += parent->GetWorldPosition(); }
	return pos;
}