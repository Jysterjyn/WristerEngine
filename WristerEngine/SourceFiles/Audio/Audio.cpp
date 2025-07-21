#include "Audio.h"
#include "D3D12Common.h"
#include <fstream>
#include <StringUtility.h>
using namespace WE;

std::string Audio::DIRECTORY_PATH = "Sounds/";

void Audio::Initialize(const std::string& fileName, bool isLoop_)
{
	Result result;
	// FilterGraphを生成
	result = CoCreateInstance(CLSID_FilterGraph,
		NULL, CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID*)&graphBuilder);

	// MediaControlインターフェース取得
	result = graphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&mediaControl);
	result = graphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID*)&mediaPosition);
	result = graphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID*)&basicAudio);

	std::string fullPath = CreateResourcePath(DIRECTORY_PATH + fileName);

	// ワイド文字列に変換
	std::wstring wfilePath = ConvertMultiByteStringToWideString(fullPath);

	// Graphを生成
	result = mediaControl->RenderFile((BSTR)wfilePath.data());

	isLoop = isLoop_;
}

void Audio::Update()
{
	if (isLoop && IsFinished()) { SetPlayPosition(0); }
	mediaPosition->get_CurrentPosition(&time);
}

bool Audio::IsFinished()
{
	double currentTime = 0, duration = 0;
	mediaPosition->get_CurrentPosition(&currentTime);
	mediaPosition->get_Duration(&duration);
	return currentTime == duration;
}