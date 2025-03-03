#pragma once
#include <string>

namespace WristerEngine
{
	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
}