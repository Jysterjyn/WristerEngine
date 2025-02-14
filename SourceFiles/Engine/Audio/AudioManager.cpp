#include "AudioManager.h"
#include "D3D12Common.h"
using namespace WristerEngine;

std::map<bool, std::list<std::unique_ptr<Audio>>> AudioManager::audios;
std::map<bool, std::list<std::unique_ptr<PointAudio>>> AudioManager::pointAudios;

void AudioManager::Initialize()
{
	// COMを初期化
	Result result = CoInitialize(NULL);
}

void AudioManager::Finalize()
{
	CoUninitialize(); // COM終了
}

Audio* AudioManager::Create(const std::string& fileName, bool isLoop)
{
	std::unique_ptr<Audio> newAudio = std::make_unique<Audio>();
	newAudio->Initialize(fileName);
	audios[isLoop].push_back(std::move(newAudio));
	return audios[isLoop].back().get();
}

PointAudio* AudioManager::CreatePointAudio(const std::string& fileName, 
	bool isLoop, const Vector3& audioPos, bool useCamera, bool usePan)
{
	std::unique_ptr<PointAudio> newAudio = std::make_unique<PointAudio>();
	newAudio->Initialize(fileName, audioPos, useCamera, usePan);
	pointAudios[isLoop].push_back(std::move(newAudio));
	return pointAudios[isLoop].back().get();
}

void AudioManager::Update()
{
	// ループ再生
	for (auto& audio : audios[true]) { if (audio->IsFinished()) { audio->SetPlayPosition(0); } }
	for (auto& audio : pointAudios[true]) { if (audio->IsFinished()) { audio->SetPlayPosition(0); } }
	// 音量の更新
	for (auto& audios_ : pointAudios) { for (auto& audio : audios_.second) { audio->Update(); } }
}