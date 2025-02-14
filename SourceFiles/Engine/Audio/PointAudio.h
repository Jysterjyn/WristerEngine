#pragma once
#include "Audio.h"
#include "Vector.h"

namespace WristerEngine
{
	// 点音源 マイクとの位置関係で音が変わる
	class PointAudio : public Audio
	{
	private:
		Vector3 audioPos;
		Vector3 micPos;
		Vector3 forward;
		bool useCamera = true;
		bool usePan = true;
		float sPressRate = 1.0f; // 原曲との音圧の比率
		long panStrength = 2500; // パンの強さ(大きいほど左右の音量差が大きくなる)

		void Initialize(const std::string& fileName, Vector3 audioPos, bool useCamera, bool usePan);
		void Update();

	public:
		friend AudioManager;
		void SetAudioPos(Vector3 pos) { audioPos = pos; }
		void SetMicPos(Vector3 pos) { micPos = pos; }
		void SetForward(Vector3 forward_) { forward = forward_; }
		void SetPanStrength(long panStrength_) { panStrength = panStrength_; }
	};
}