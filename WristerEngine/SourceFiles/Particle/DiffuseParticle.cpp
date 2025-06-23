#include "DiffuseParticle.h"
#include "Random.h"
#include <cassert>
using namespace WristerEngine;

//void DiffuseParticle::Add(const AddProp& particleProp)
//{
//	for (unsigned short i = 0; i < particleProp.addNum; i++)
//	{
//		particles.emplace_front();
//		Particle& p = particles.front();
//		p.position = RandomVector(particleProp.posRange) + particleProp.posOffset;
//		p.velocity = RandomVector(particleProp.velRange) + particleProp.velOffset;
//		p.accel = RandomVector(particleProp.accRange) + particleProp.accOffset;
//		p.frame = particleProp.lifeTime;
//		p.s_scale = particleProp.startScale;
//		p.e_scale = particleProp.endScale;
//		p.parent = particleProp.parent;
//	}
//}
//
//void DiffuseParticle::Update()
//{
//	particles.remove_if([](Particle& particle) { return particle.frame.Update(); });
//}

void WristerEngine::DiffuseParticle::Initialize(const BaseParticleProp& prop)
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

void WristerEngine::DiffuseParticle::Update()
{
	position += velocity;
	velocity += accel;
	scale = Lerp(s_scale, e_scale, frame.GetTimeRate());
	color = ColorRGBA::White();
}
