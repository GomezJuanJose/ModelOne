#include "plpch.h"
#include "Audio.h"

#include "soloud_wav.h"

namespace Primitivo {
  
	SoLoud::Soloud AudioCommand::s_gSoloud;


	void AudioCommand::InitAudioEngine() {
		
		s_gSoloud.init();
	}

   

	void AudioCommand::PlayAudio(AssetRef<SoundWav> sound, bool loop, float volume, float pan) {
		sound->setLooping(loop);
		s_gSoloud.play(*sound,volume,pan);
	}

	void AudioCommand::StopAudio(AssetRef<SoundWav> sound) {
		
		s_gSoloud.stop(sound->mAudioSourceID);
	}

	void AudioCommand::StopAllAudio() {
		s_gSoloud.stopAll();
	}

	void AudioCommand::PauseAudio(AssetRef<SoundWav> sound) {
		s_gSoloud.setPause(sound->mAudioSourceID, true);
	}

	void AudioCommand::ResumeAudio(AssetRef<SoundWav> sound) {
		s_gSoloud.setPause(sound->mAudioSourceID, false);
	}

	void AudioCommand::SetVolume(AssetRef<SoundWav> sound, float volume) {
		sound->setVolume(volume);
	}

	void AudioCommand::SetPan(AssetRef<SoundWav> sound, float pan) {
		s_gSoloud.setPan(sound->mAudioSourceID, pan);
	}

	void AudioCommand::DeinitAudioEngine() {
		s_gSoloud.deinit();

	}



}
