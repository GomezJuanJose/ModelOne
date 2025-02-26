#pragma once

namespace SoLoud {
	class Wav;
	class Soloud;
}


namespace ModelOne {

	using SoundWav = SoLoud::Wav;

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