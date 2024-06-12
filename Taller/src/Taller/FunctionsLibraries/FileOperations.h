#pragma once
#include <fstream>

namespace Taller {
	class FileOperations {
	public:
		static std::string ReadFile(const std::string& filePath) {// TODO Redo with a proper file system with virtual files

			TL_PROFILE_RENDERER_FUNCTION();

			std::string result;
			std::ifstream in(filePath, std::ios::in | std::ios::binary);

			if (in) {
				in.seekg(0, std::ios::end);
				result.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else {
				TL_LOG_ERROR("Could not open file %s", filePath);
			}

			return result;
		}
	};
}