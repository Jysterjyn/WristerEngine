#include "DiffuseParticle.h"
#include "Random.h"
#include <cassert>
using namespace WE;

void DiffuseParticle::Initialize(const BaseParticleProp& prop)
{
	const Prop* addProp = static_cast<const Prop*>(&prop);
	assert(addProp);
	position = RandomVector(addProp->posRange) + addProp->posOffset;
	velocity = RandomVector(addProp->velRange) + addProp->velOffset;
	accel = RandomVector(addProp->accRange) + addProp->accOffset;
	frame = addProp->lifeTime;
	s_scale = addProp->startScale;
	e_scale = addProp->endScale;
	parent = addProp->parent;
}

void DiffuseParticle::Update()
{
	position += velocity;
	velocity += accel;
	scale = Lerp(s_scale, e_scale, frame.GetTimeRate());
	if (frame.Update()) { isDestroy = true; }
	color = ColorRGBA::White();
}
