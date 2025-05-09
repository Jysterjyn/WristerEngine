#include "PointAudio.h"
#include "ModelManager.h"
#include "BaseCamera.h"
#include <cmath>
using namespace WristerEngine;

void PointAudio::Initialize(const std::string& fileName, Vector3 audioPos_, bool useCamera_, bool usePan_)
{
	Audio::Initialize(fileName);
	useCamera = useCamera_;
	audioPos = audioPos_;
	usePan = usePan_;
}

void PointAudio::Update()
{
	if (useCamera)
	{
		_3D::BaseCamera* vp = _3D::ModelManager::GetInstance()->GetCamera();
		micPos = vp->eye;
		forward = Normalize(vp->target - vp->eye);
	}
	// マイクから音源の距離
	Vector3 toMic = micPos - audioPos;
	float dis = toMic.Length();
	// 距離が1未満だと、音源の音量より大きくなるので調整
	dis = max(dis, 1.0f);
	// 距離減衰
	float dic = 20.0f * std::log10f(dis); // デシベルの計算式(20 * log10(d)より)
	// dic最小値
	const float MIN_VOLUME = -10000.0f;
	dic = max(dic, MIN_VOLUME);
	SetVolume(-(long)(dic * 100.0f));
	// 音源の左右移動(パン)
	if (!usePan) { return; }
	// y軸成分を消した2成分軸で外積を取る
	Vector2 dVec[2];
	dVec[0] = { forward.x,forward.z };
	toMic.Normalize();
	dVec[1] = { toMic.x,toMic.z };
	float cross = Cross(dVec[0], dVec[1]);
	SetBalance((long)(cross * (float)panStrength));
}