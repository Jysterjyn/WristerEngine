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
	Audio* newAudio = static_cast<Audio*>(audios.Add(std::make_unique<Audio>(fileName, isLoop)));
	return newAudio;
}

void AudioManager::PlaySE(const std::string& fileName)
{
	Audio* newAudio = Create(fileName);
	newAudio->isSE = true;
	newAudio->Play();
}

void AudioManager::Update()
{
	audios.Update();
}