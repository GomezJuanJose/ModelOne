#pragma once





namespace ModelOne {
	class Texture2D;

	class Texture2DLibrary {
	public:
		void Add(const std::string& name, const AssetRef<Texture2D>& texture);
		AssetRef<Texture2D> Load(const std::string& name, const std::string& filePath);

		AssetRef<Texture2D> Get(const std::string& name);

		bool Exist(const std::string& name) const;

	private:
		std::unordered_map<std::string, AssetRef<Texture2D>> m_Textures;
	};
}
