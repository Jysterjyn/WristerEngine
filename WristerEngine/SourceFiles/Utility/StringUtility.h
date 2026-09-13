#pragma once
#include <string>
#include <WristerEngineUtility.h>

namespace WristerEngine
{
	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	std::wstring ConvertMultiByteStringToWideString(CR<std::string> mString);

	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(CR<std::string> path);
}