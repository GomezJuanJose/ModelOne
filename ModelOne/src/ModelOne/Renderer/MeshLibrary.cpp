#include "mopch.h"
#include "MeshLibrary.h"
#include "ModelOne/Renderer/Buffer.h"
#include "ModelOne/FunctionsLibraries/FileOperations.h"
#include "ModelOne/FunctionsLibraries/StringOperations.h"


#include <sstream>

namespace ModelOne {


	AssetRef<VertexArray>& MeshLibrary::Load(const std::string& name, const std::string& filePath) {
		
		AssetRef<VertexArray> vertexArray = VertexArray::Create();
		
		std::string file_text = FileOperations::ReadFile(filePath);



		//Gets all attributes from the vertex
		std::vector<glm::vec3> vertex_coord = RetrieveObjAttribute(file_text, "v");
		std::vector<glm::vec3> vertex_texture = RetrieveObjAttribute(file_text, "vt");
		std::vector<glm::vec3> vertex_normal = RetrieveObjAttribute(file_text, "vn");




		//Gets all triangle and indices data
		std::istringstream file(file_text);

		std::vector<float> vertices;
		std::vector<uint32_t> indices;


		std::string line;
		int index = 0;
		while (std::getline(file, line)) {
			std::vector<std::string> result = SplitString(line, " ");

			if (result[0] == "f") {
				for (int i = 1; i < result.size(); i++) {
					std::vector<std::string> triangle = SplitString(result[i], "/");
					int coord_index = std::atoi(triangle[0].c_str())-1;
					vertices.push_back(vertex_coord[coord_index].x);
					vertices.push_back(vertex_coord[coord_index].y);
					vertices.push_back(vertex_coord[coord_index].z);

					int text_index = std::atoi(triangle[1].c_str())-1;
					vertices.push_back(vertex_texture[text_index].s);
					vertices.push_back(vertex_texture[text_index].t);

					int normal_index = std::atoi(triangle[2].c_str())-1;
					vertices.push_back(vertex_normal[normal_index].x);
					vertices.push_back(vertex_normal[normal_index].y);
					vertices.push_back(vertex_normal[normal_index].z);
					
					indices.push_back(index);
					index++;
				}
			}

		}

		
		
		//Create vertex buffer and layout buffer

		AssetRef<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float))); //This multiplication is because the vector itself due the pointer occupies a little more bytes and due to that th meshes were not renderer
		ModelOne::BufferLayout triangleLayout = {
			{ModelOne::ShaderDataType::Float3, "a_Position"},
			{ModelOne::ShaderDataType::Float2, "a_Texcoord"},
			{ModelOne::ShaderDataType::Float3, "a_Normal"}
			
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
		MO_ASSERT(Exist(name), "Mesh not found");
		return m_Meshes[name];
	}

	bool MeshLibrary::Exist(const std::string& name) const {
		return m_Meshes.find(name) != m_Meshes.end();
	}

	void MeshLibrary::Add(const std::string& name, const AssetRef<VertexArray>& meshData) {
		MO_ASSERT(!Exist(name), "Mesh already exists!");
		m_Meshes[name] = meshData;
	}





	std::vector<glm::vec3> MeshLibrary::RetrieveObjAttribute(std::string& file_text, std::string attribute) {
		std::istringstream file(file_text);
		std::vector<glm::vec3> attributes;

		
		std::string line;
		while (std::getline(file, line)) {
			std::vector<std::string> result = SplitString(line, " ");

			if (result[0] == attribute) {
				glm::vec3 data = {0, 0, 0};
	
				data.x = std::atof(result[1].c_str());
				data.y = std::atof(result[2].c_str());
				if (result.size() > 3) { // If there is has the prefix and 3 values
					data.z = std::atof(result[3].c_str());
				}

				attributes.push_back(data);
			}
			
		}

		
		return attributes;
	}
}