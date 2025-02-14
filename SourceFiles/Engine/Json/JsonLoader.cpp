#include "JsonLoader.h"
#include <fstream>
#include <cassert>
using namespace WristerEngine;

const std::string JsonLoader::DEFAULT_BASE_DIRECTORY = "Resources/JsonFiles/";

nlohmann::json WristerEngine::JsonLoader::LoadJson(const std::string& fileName)
{	
	// フルパス
	const std::string fullpath = DEFAULT_BASE_DIRECTORY + fileName + ".json";
	std::ifstream file; // ファイルストリーム
	// ファイルを開く
	file.open(fullpath);
	// 読み込み失敗の時
	if (file.fail()) { assert(0); }

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;
	// 解凍
	file >> deserialized;
	file.close();
	return deserialized;
}
