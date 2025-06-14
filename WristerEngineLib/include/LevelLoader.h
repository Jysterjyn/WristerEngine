#pragma once
#include "JsonLoader.h"
#include "Vector.h"
#include "Easing.h"

namespace WristerEngine
{
	struct ObjectData
	{
		std::string name;
		float posX;
		Vector2 size;
		Vector2 e_eyePos;
		Vector2 e_gaugePos;
		int e_attackInterval;
		int e_attackTime;
		std::array<int, 2> e_angleRange{};
		bool e_flip;
		float e_hp = 100;
		int e_beamEasingTime = 120;
		WristerEngine::Easing::Type e_beamEasingType = WristerEngine::Easing::Type::Linear;
	};

	class LevelLoader : public WristerEngine::JsonLoader
	{
	public:
		std::vector<ObjectData> LoadLevel(const std::string& fileName);
	};
}