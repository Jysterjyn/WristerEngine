#include "LevelLoader.h"
using namespace WristerEngine;

std::vector<ObjectData> LevelLoader::LoadLevel(const std::string& fileName)
{
	std::vector<ObjectData> objects;
	auto loaded = LoadJson(fileName);
	for (nlohmann::json& object : loaded["objects"])
	{
		ObjectData obj{};
		obj.name = object["name"];
		obj.posX = object["positionX"];
		obj.size.x = object["size"][0];
		obj.size.y = object["size"][1];
		if(object.contains("eyePos"))
		{
			obj.e_eyePos.x = object["eyePos"][0];
			obj.e_eyePos.y = object["eyePos"][1];
		}
		if (object.contains("gaugePos"))
		{
			obj.e_gaugePos.x = object["gaugePos"][0];
			obj.e_gaugePos.y = object["gaugePos"][1];
		}
		if (object.contains("attackInterval"))
		{
			obj.e_attackInterval = object["attackInterval"];
		}
		if (object.contains("attackTime"))
		{
			obj.e_attackTime = object["attackTime"];
		}
		if (object.contains("angleRange"))
		{
			obj.e_angleRange = object["angleRange"];
		}
		if (object.contains("flip"))
		{
			obj.e_flip = object["flip"];
		}
		if (object.contains("hp"))
		{
			obj.e_hp = object["hp"];
		}
		if (object.contains("beamEasingTime"))
		{
			obj.e_beamEasingTime = object["beamEasingTime"];
		}
		if (object.contains("beamEasingType"))
		{
			if (object["beamEasingType"] == "InOutQuint")
			{
				obj.e_beamEasingType = WristerEngine::Easing::Type::EaseInOutQuint;
			}
			else if (object["beamEasingType"] == "OutElastic")
			{
				obj.e_beamEasingType = WristerEngine::Easing::Type::OutElastic;
			}
		}
		objects.push_back(obj);
	}
	return objects;
}
