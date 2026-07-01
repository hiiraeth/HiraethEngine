#ifndef HIRAETHENGINE_UTILS_H
#define HIRAETHENGINE_UTILS_H

#include <filesystem>
#include <vector>

#define LOG_INFO(msg)    std::cout << "[INFO] "    << msg;
#define LOG_WARNING(msg) std::cerr << "[WARNING] " << msg;
#define LOG_ERROR(msg)   std::cerr << "[ERROR] "   << msg;

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals  | aiProcess_ForceGenNormals | aiProcess_JoinIdenticalVertices)

namespace HIRAETHENGINE_UTILS
{
	namespace fs = std::filesystem;

	inline std::vector<std::string> GetSceneFiles()
	{
		std::vector<std::string> sceneFiles;
		std::string directory = "../resources/scenes";

		for (const auto& entry : fs::directory_iterator(directory))
		{
			std::string path = entry.path();
			sceneFiles.push_back(path);
		}

		return sceneFiles;
	}

	inline unsigned int GetNextSceneFileNum()
	{
		std::vector<std::string> files = GetSceneFiles();
		if (files.empty()) return 1;

		unsigned int highest = 0;
		for (const auto& file : files)
		{
			std::string name = fs::path(file).stem().string(); // e.g. "scene_002"
			std::string numStr = name.substr(name.find_last_of('_') + 1); // "002"
			unsigned int num = std::stoi(numStr);
			if (num > highest) highest = num;
		}
		return highest + 1;
	}

	inline bool CheckIfFileExists(std::string path)
	{
		std::vector<std::string> files = GetSceneFiles();

		for (const auto& file : files)
		{
			if (file == path)
				return true;
		}

		return false;
	}
}


#endif //HIRAETHENGINE_UTILS_H