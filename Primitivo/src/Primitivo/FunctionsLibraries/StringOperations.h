#pragma once
#include <string>
#include <vector>
namespace Primitivo {
	
	std::vector<std::string> SplitString(std::string& string, const std::string& delimiter) {

		std::vector<std::string> result;

		//Removes prefix
		int position = string.find(delimiter);

		while (position != std::string::npos) {
			result.push_back(string.substr(0, position));
			string.erase(0, position + 1);
			position = string.find(delimiter);
		}

		//Gets last value
		position = string.length();
		result.push_back(string.substr(0, position));

		
		return result;
	}
}