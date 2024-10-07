#pragma once

#include "Taller/Core/Core.h"

namespace SoLoud {
	class Wav;
	class Soloud;
}

namespace Taller {

	using SoundWav = SoLoud::Wav;

	class AudioCommand {
	public:
		static void InitAudioEngine();
		static void PlayAudio(AssetRef<SoundWav> sound, bool loop, float volume, float pan);
		static void StopAudio(AssetRef<SoundWav> sound);
		static void StopAllAudio();
		static void PauseAudio(AssetRef<SoundWav> sound);
		static void ResumeAudio(AssetRef<SoundWav> sound);
		static void SetVolume(AssetRef<SoundWav> sound, float volume);
		static void SetPan(AssetRef<SoundWav> sound, float pan);
		static void DeinitAudioEngine();

	private:
		static SoLoud::Soloud s_gSoloud;

	};

	class SoundLibrary {
	public:
		void Add(const std::string& name, const AssetRef<SoundWav>& sound);
		AssetRef<SoundWav> Load(const std::string& name, const std::string& filePath);

		AssetRef<SoundWav> Get(const std::string& name);

		bool Exist(const std::string& name) const;

	private:
		std::unordered_map<std::string, AssetRef<SoundWav>> m_Sounds;
	};
}

