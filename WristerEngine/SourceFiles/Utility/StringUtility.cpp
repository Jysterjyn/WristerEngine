#include "StringUtility.h"
#include <vector>
#include <Windows.h>

std::wstring WristerEngine::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ���C�h������ɕϊ������ۂ̕�����o�b�t�@�T�C�Y���v�Z
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	// ���C�h������ɕϊ�
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
