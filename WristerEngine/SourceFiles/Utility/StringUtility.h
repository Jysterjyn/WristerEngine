#pragma once
#include <string>

namespace WristerEngine
{
	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�
	/// </summary>
	/// <param name="mString">�}���`�o�C�g������</param>
	/// <returns>���C�h������</returns>
	std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
}