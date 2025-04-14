#include "StringUtility.h"
#include <vector>
#include <Windows.h>

std::wstring WristerEngine::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	// ワイド文字列に変換
	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, wfilePath.data(), filePathBufferSize);

	return wfilePath.data();
}

std::string WristerEngine::ExtractFileName(const std::string& path)
{
	size_t pos1 = path.rfind('\\');
	if (pos1 != std::string::npos) { return path.substr(pos1 + 1, path.size() - pos1 - 1); }

	pos1 = path.rfind('/');
	if (pos1 != std::string::npos) { return path.substr(pos1 + 1, path.size() - pos1 - 1); }

	return path;
}
