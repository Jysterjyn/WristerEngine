#include <WristerEngineUtility.h>
#include <Windows.h>

using namespace WE;

ListObject* uList::Add(uPtr<ListObject> obj)
{
	obj->Initialize();
	push_back(std::move(obj));
	return back().get();
}

void uList::Update()
{
	remove_if([&](CR<uPtr<ListObject>> item) { return item->Remove(); });
	for (auto& item : *this) { item->Update(); }
}

std::wstring WE::ConvertMultiByteStringToWideString(CR<std::string> mString)
{
	// ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	// ワイド文字列に変換
	std::vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, wfilePath.data(), filePathBufferSize);

	return wfilePath.data();
}

std::string WE::ExtractFileName(CR<std::string> path)
{
	size_t pos1 = path.rfind('\\');
	if (pos1 != std::string::npos) { return path.substr(pos1 + 1, path.size() - pos1 - 1); }

	pos1 = path.rfind('/');
	if (pos1 != std::string::npos) { return path.substr(pos1 + 1, path.size() - pos1 - 1); }

	return path;
}
