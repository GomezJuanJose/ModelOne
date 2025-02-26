#pragma once



namespace SoLoud {
	class Wav;
	class Soloud;
}

namespace ModelOne {

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
}

