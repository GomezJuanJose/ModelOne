#include "plpch.h"
#include "SoundLibrary.h"

#include "soloud_wav.h"
#include "Primitivo/Core/Core.h"

namespace Primitivo {
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