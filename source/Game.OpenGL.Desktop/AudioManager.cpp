#include "pch.h"
#include "AudioManager.h"
#include <GLFW/glfw3.h>
#include "Game.h"

using namespace FieaGameEngine;
using namespace irrklang;

AudioManager::AudioManager()
{
	SoundEngine = createIrrKlangDevice();
	SoundEngine->setSoundVolume(.25f);
	PlayMusic(m_DefaultMusicStringLiteral, defaultVolume);
	Event<PlaySoundEvent>::Subscribe(*this);
}

AudioManager::~AudioManager()
{
	Event<PlaySoundEvent>::Unsubscribe(*this);
}

void AudioManager::PlaySoundFX(const std::string& fileName, float volume, bool isLooping)
{
	/*if (currentSoundEffect == nullptr || currentSoundEffect->isFinished())*/
	currentSoundEffect = SoundEngine->play2D(fileName.c_str(), isLooping, false, true);
	currentSoundEffect->setVolume(volume);
}

void AudioManager::PlayMusic(const std::string& fileName, float volume, bool isLooping)
{
	if (currentMusic == nullptr || currentMusic->isLooped())
	{
		if (currentMusic)
		{
			currentMusic->stop();
		}
		currentMusic = SoundEngine->play2D(fileName.c_str(), isLooping, false, true);
		currentMusic->setVolume(volume);
	}
}

void AudioManager::Notify(const EventPublisher& publisher)
{
	const Event<PlaySoundEvent>* playSoundEvent = publisher.As<Event<PlaySoundEvent>>();
	if (playSoundEvent)
	{
		auto& payload = playSoundEvent->Message();
		payload.IsMusic ? PlayMusic(payload.fileName, payload.volume, payload.IsLooping) : PlaySoundFX(payload.fileName, payload.volume, payload.IsLooping);
	}
}

const Vector<Signature> AudioManager::Signatures()
{
	return Vector<Signature>
	{

	};
}