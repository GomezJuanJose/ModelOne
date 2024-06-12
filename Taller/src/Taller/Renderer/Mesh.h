#pragma once
#include "Taller/Renderer/VertexArray.h"


namespace Taller {

	class MeshLibrary {

	public:
		AssetRef<VertexArray>& Load(const std::string& name, const std::string& filePath);

		AssetRef<VertexArray>& Get(const std::string& name);
		bool Exist(const std::string& name) const;

	private:
		void Add(const std::string& name, const AssetRef<VertexArray>& meshData);

	private:
		std::unordered_map<std::string, AssetRef<VertexArray>> m_Meshes;

		std::unordered_map<int, std::string> ParseFile(const std::string& filePath);
		std::vector<float> ParseFloatSection(const std::string& section);
		std::vector<uint32_t> ParseIntSection(const std::string& section);
	};
}

