#include "FollowCamera.h"
using namespace WE;
using namespace _3D;

FollowCamera::FollowCamera(const Prop* prop)
{
	if (prop) { target = prop->target; }
}

void FollowCamera::VirtualUpdate()
{
}