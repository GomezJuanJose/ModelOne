#include "tlpch.h"
#include "Audio.h"
#include "soloud_wav.h"

namespace Taller {
  

	//TODO arreglar el assert
	
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




	void SoundLibrary::Add(const std::string& name, const AssetRef<SoundWav>& sound) {
		TL_ASSERT(!Exist(name), "Sound already exists!!");
		m_Sounds[name] = sound;
	}
	AssetRef<SoundWav> SoundLibrary::Load(const std::string& name, const std::string& filePath) {
		AssetRef <SoundWav> sound = CreateAssetRef<SoundWav>();
		sound->load(filePath.c_str());
		Add(name, sound);
		return sound;
	}
	AssetRef<SoundWav> SoundLibrary::Get(const std::string& name) {
		TL_ASSERT(Exist(name), "Sound not found!!");
		return m_Sounds[name];
	}
	bool SoundLibrary::Exist(const std::string& name) const {
		return m_Sounds.find(name) != m_Sounds.end();
	}
}
