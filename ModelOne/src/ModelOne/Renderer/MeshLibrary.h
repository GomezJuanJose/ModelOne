#pragma once
#include "ModelOne/Renderer/VertexArray.h"

#include "ModelOne/Core/Core.h"

#include <glm/glm.hpp>

namespace ModelOne {


	class MeshLibrary {

	public:
		AssetRef<VertexArray>& Load(const std::string& name, const std::string& filePath);

		AssetRef<VertexArray>& Get(const std::string& name);
		bool Exist(const std::string& name) const;

	private:
		void Add(const std::string& name, const AssetRef<VertexArray>& meshData);

	private:
		std::unordered_map<std::string, AssetRef<VertexArray>> m_Meshes;


		std::vector<glm::vec3> RetrieveObjAttribute(std::string& file_text, std::string attribute);

	};
}

