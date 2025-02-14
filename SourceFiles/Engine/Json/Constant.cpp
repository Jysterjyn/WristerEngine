#include "Constant.h"
#include "Color.h"
#include <fstream>
#include "BitMapNumber.h"

using namespace WristerEngine;

Constant* Constant::GetInstance()
{
	static Constant instance;
	return &instance;
}

void WristerEngine::Constant::LoadInt()
{
	if (deserialized.contains("int"))
	{
		// 定数リスト取得とインスタンス生成(int)
		const auto& list = deserialized["int"].get<std::map<std::string, int>>();
		for (const auto& val : list) { constants[val.first] = new int(val.second); }
	}

	if (deserialized.contains("int[]"))
	{
		// 定数リスト取得とインスタンス生成(int配列)
		const auto& list = deserialized["int[]"].get<std::map<std::string, std::vector<int>>>();
		for (const auto& val : list) 
		{
			constants[val.first] = new std::vector<int>(val.second);
		}
	}
}

void WristerEngine::Constant::LoadFloat()
{
	if (!deserialized.contains("float")) { return; }
	// 定数リスト取得とインスタンス生成(float)
	const auto& list = deserialized["float"].get<std::map<std::string, float>>();
	for (const auto& val : list) { constants[val.first] = new float(val.second); }
}

void WristerEngine::Constant::LoadDouble()
{
	if (!deserialized.contains("double")) { return; }
	// 定数リスト取得とインスタンス生成(float)
	const auto& list = deserialized["double"].get<std::map<std::string, double>>();
	for (const auto& val : list) { constants[val.first] = new double(val.second); }
}

void WristerEngine::Constant::LoadVector()
{
	if (deserialized.contains("Vector2"))
	{
		// 定数リスト取得とインスタンス生成(Vector2)
		const auto& list = deserialized["Vector2"].get<std::map<std::string, std::vector<float>>>();
		for (const auto& val : list) { constants[val.first] = new Vector2(val.second[0], val.second[1]); }
	}

	if (deserialized.contains("Vector3"))
	{
		// 定数リスト取得とインスタンス生成(Vector3)
		const auto& list = deserialized["Vector3"].get<std::map<std::string, std::vector<float>>>();
		for (const auto& val : list) { constants[val.first] = new Vector3(val.second[0], val.second[1], val.second[2]); }
	}
}

void WristerEngine::Constant::LoadColor()
{
	if (deserialized.contains("ColorRGB"))
	{
		// 定数リスト取得とインスタンス生成(ColorRGB)
		const auto& list = deserialized["ColorRGB"].get<std::map<std::string, std::vector<float>>>();
		for (const auto& val : list) { constants[val.first] = new ColorRGB(val.second[0], val.second[1], val.second[2]); }
	}

	if (deserialized.contains("ColorRGBA"))
	{
		// 定数リスト取得とインスタンス生成(ColorRGBA)
		const auto& list = deserialized["ColorRGBA"].get<std::map<std::string, std::vector<float>>>();
		for (const auto& val : list) { constants[val.first] = new ColorRGBA(val.second[0], val.second[1], val.second[2], val.second[3]); }
	}
}

void WristerEngine::Constant::LoadStruct()
{
	if (deserialized.contains("BitMapProp"))
	{
		const auto& list = deserialized["BitMapProp"].get<std::map<std::string, std::vector<float>>>();
		for (const auto& val : list) 
		{
			_2D::BitMapProp* bProp = new _2D::BitMapProp;
			bProp->rectSize = { val.second[0],val.second[1] };
			bProp->size = { val.second[2],val.second[3] };
			bProp->pos = { val.second[4],val.second[5] };
			bProp->digit = (int)val.second[6];
			constants[val.first] = bProp;
		}
	}
}

void Constant::LoadConstants()
{
	deserialized = LoadJson("constant");

	LoadInt();
	LoadFloat();
	LoadDouble();
	LoadVector();
	LoadColor();
	LoadStruct();
}

void WristerEngine::Constant::Finalize()
{
	for (auto& p : constants)
	{
		// メモリ解放
		delete p.second;
		p.second = nullptr;
	}
	constants.clear();
}