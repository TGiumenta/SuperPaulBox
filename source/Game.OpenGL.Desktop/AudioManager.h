#pragma once
#include "Entity.h"
#include "HashMap.h"
#include "Input.h"
#include "irrKlang.h"

using namespace FieaGameEngine;
using namespace irrklang;

class AudioManager final : public EventSubscriber
{

public:
	AudioManager();
	~AudioManager();

	virtual void Notify(const class EventPublisher&) override;

	static const Vector<Signature> Signatures();

	void PlaySoundFX(const std::string& fileName, float volume = defaultVolume, bool isLooping = false);
	void PlayMusic(const std::string& fileName, float volume = defaultVolume, bool isLooping = true);

	struct PlaySoundEvent {
		const std::string fileName;
		bool IsMusic;
		bool IsLooping;
		float volume = defaultVolume;
	};

	inline const static float defaultVolume = 0.25f;

private:
	ISoundEngine* SoundEngine;
	ISound* currentMusic = nullptr;
	ISound* currentSoundEffect = nullptr;

	// Used for debug key
	//HashMap<KeyboardEvent, InputFunction> m_InputActions;
	const std::string m_DefaultMusicStringLiteral = "8BitHipHopRapInstrumentalMonkeyIsland.mp3";
};
