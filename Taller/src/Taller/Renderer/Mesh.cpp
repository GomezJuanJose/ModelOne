#include "tlpch.h"
#include "Mesh.h"
#include "Taller/Renderer/Buffer.h"
#include "Taller/FunctionsLibraries/FileOperations.h"

#include <glm/glm.hpp>

namespace Taller {


	AssetRef<VertexArray>& MeshLibrary::Load(const std::string& name, const std::string& filePath) {
		
		AssetRef<VertexArray> vertexArray = VertexArray::Create();


		//Divide the file in data sections and stores it into a map
		std::unordered_map<int, std::string> fileSections = ParseFile(filePath);

		//Parse the first section, the vertex data
		std::vector<float> vertices = ParseFloatSection(fileSections[1]);
		
		//Parse the second section, the indices
		std::vector<uint32_t> indices = ParseIntSection(fileSections[2]);


		AssetRef<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float))); //This multiplication is because the vector itself due the pointer occupies a little more bytes and due to that th meshes were not renderer
		Taller::BufferLayout triangleLayout = {
			{Taller::ShaderDataType::Float3, "a_Position"},
			{Taller::ShaderDataType::Float3, "a_Normal"},
			{Taller::ShaderDataType::Float2, "a_Texture"}
		};
		vertexBuffer->SetLayout(triangleLayout);
		vertexArray->AddVertexBuffer(vertexBuffer);


		AssetRef<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(&indices[0], vertices.size() * sizeof(uint32_t) / sizeof(uint32_t)));
		vertexArray->SetIndexBuffer(indexBuffer);


		Add(name, vertexArray);

		return vertexArray;

	}

	AssetRef<VertexArray>& MeshLibrary::Get(const std::string& name) {
		TL_ASSERT(Exist(name), "Mesh not found");
		return m_Meshes[name];
	}

	bool MeshLibrary::Exist(const std::string& name) const {
		return m_Meshes.find(name) != m_Meshes.end();
	}

	void MeshLibrary::Add(const std::string& name, const AssetRef<VertexArray>& meshData) {
		TL_ASSERT(!Exist(name), "Mesh already exists!");
		m_Meshes[name] = meshData;
	}




	static int MeshSectionFromString(const std::string& type) {
		if (type == "vertex data") {
			return 1;
		}
		else if (type == "indices") {
			return 2;
		}

		TL_ASSERT(false, "Unknown mesh section!");
		return 0;
	}

	std::unordered_map<int, std::string> MeshLibrary::ParseFile(const std::string& filePath) {
		std::string source = FileOperations::ReadFile(filePath);

		
		std::vector<float> indexBuffer;

		std::unordered_map<int, std::string> meshSections;
		const char* typeToken = "#section";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			TL_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			TL_ASSERT(MeshSectionFromString(type), "Invalid section for the mesh, at the moment is not supported!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			meshSections[MeshSectionFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return meshSections;
	}

	std::vector<float> MeshLibrary::ParseFloatSection(const std::string& section) {

		std::vector<float> buffer;

		std::istringstream issVertex(section);

		for (std::string line; std::getline(issVertex, line);) {

			std::string output;
			std::stringstream ss(line);
			std::vector<std::string>rawValues;

			while (std::getline(ss, output, ' ')) {
				if (output != "\r" && output != "") {
					buffer.push_back(std::stof(output));
				}
			}

		}

		return buffer;
	}

	std::vector<uint32_t> MeshLibrary::ParseIntSection(const std::string& section) {

		std::vector<uint32_t> buffer;

		std::istringstream issVertex(section);

		for (std::string line; std::getline(issVertex, line);) {

			std::string output;
			std::stringstream ss(line);
			std::vector<std::string>rawValues;

			while (std::getline(ss, output, ' ')) {
				if (output != "\r" && output != "") {
					buffer.push_back(std::stoul(output));
				}
			}

		}

		return buffer;
	}
}