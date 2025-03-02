#include "DiffuseParticle.h"
#include "Random.h"
using namespace WristerEngine;

void DiffuseParticle::Add(const AddProp& particleProp)
{
	for (unsigned short i = 0; i < particleProp.addNum; i++)
	{
		particles.emplace_front();
		Particle& p = particles.front();
		p.position = RandomVector(particleProp.posRange) + particleProp.posOffset;
		p.velocity = RandomVector(particleProp.velRange) + particleProp.velOffset;
		p.accel = RandomVector(particleProp.accRange) + particleProp.accOffset;
		p.frame = particleProp.lifeTime;
		p.s_scale = particleProp.startScale;
		p.e_scale = particleProp.endScale;
		p.parent = particleProp.parent;
	}
}

void DiffuseParticle::Update()
{
	particles.remove_if([](Particle& particle) { return particle.frame.Update(); });
}