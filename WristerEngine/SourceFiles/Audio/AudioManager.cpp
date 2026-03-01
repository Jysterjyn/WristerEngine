#include "AudioManager.h"
#include "D3D12Common.h"
using namespace WE;

AudioManager* AudioManager::GetInstance()
{
	static AudioManager instance;
	return &instance;
}

Audio* AudioManager::Create(const std::string& fileName, bool isLoop)
{
	std::unique_ptr<Audio> newAudio = std::make_unique<Audio>();
	newAudio->Initialize(fileName, isLoop);
	audios.push_back(std::move(newAudio));
	return audios.back().get();
}

void AudioManager::PlaySE(const std::string& fileName)
{
	Audio* newAudio = Create(fileName);
	newAudio->isSE = true;
	newAudio->Play();
}

void AudioManager::Update()
{
	audios.remove_if([](auto& audio) { return audio->isSE && audio->IsFinished(); });

	// 更新処理
	for (auto& audio : audios) { audio->Update(); }
}