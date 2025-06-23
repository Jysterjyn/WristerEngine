#include "DirectionalParticle.h"
#include "Quaternion.h"
#include <cassert>
using namespace WE;

void DirectionalParticle::Initialize(const BaseParticleProp& prop)
{
	const Prop* addProp = static_cast<const Prop*>(&prop);
	assert(addProp);
	start = addProp->start;
	end = addProp->end;
	scale = addProp->scale;
	splitNum = addProp->splitNum;
	angle = addProp->angle;
	radius = addProp->radius;
	frame = addProp->lifeTime;
	ComputeControlPoints();
}

void DirectionalParticle::ComputeControlPoints()
{
	// 回転軸を求める
	Vector3 axis = Normalize(end - start);
	// Quaternion生成
	Quaternion rotQ = MakeAxisAngle(axis, angle);
	// 制御点の追加
	controlPoints.push_back(start);
	// 制御点を線形補間で計算
	for (size_t i = 0; i < splitNum; i++)
	{
		// axisの分割
		Vector3 controlPoint = Lerp(start, end, (float)(i + 1) / (float)(splitNum + 1));
		// 制御点をy軸方向にradiusだけ移動させる
		controlPoint.y += radius;
		// Quaternionは原点中心に回転させるので、回転軸の始点を原点とする
		controlPoint -= start;
		// 制御点をQuaternionで回転させる
		controlPoint = RotateVector(controlPoint, rotQ);
		// 平行移動を相殺
		controlPoint += start;
		controlPoints.push_back(controlPoint);
	}
	controlPoints.push_back(end);
}

void DirectionalParticle::Update()
{
	position = BezierCurve(controlPoints, frame.GetRemainTimeRate());
	if (frame.Update()) { isDestroy = true; }
}
