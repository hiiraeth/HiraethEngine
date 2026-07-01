//
// Created by hiiraeth on 2/15/26.
//

#ifndef HIRAETHENGINE_TEXTURE_H
#define HIRAETHENGINE_TEXTURE_H
#include <string>
#include <vector>

#include "glad/glad.h"

class Texture
{
public:
	struct TextureInfo
	{
		std::string tag;
		unsigned int ID;
	};

	Texture(GLenum texTarget, std::string &filePath);
	Texture(GLenum texTarget, std::string &filePath, std::string texTag);

	Texture();

	GLuint LoadTexture();
	unsigned int LoadCubemap(std::vector<std::string> faces);
	bool LoadTexture(GLenum texTarget, const std::string &filePath, const std::string &tag);

	int FindTextureID(std::string tag) const;

	void BindGLTextures() const;

	void LoadTextures();

	std::vector<TextureInfo> GetTextures();


private:
	bool LoadSTBIData(const std::string &filePath, int width, int height, int channels, unsigned char *imageData);

	GLenum m_TextureTarget;
	std::string m_FilePath;

	GLuint m_TextureID;
	std::string m_TextureTag;

	std::vector<TextureInfo> m_TextureIDs;
	int m_LoadedTextures = 0;
};

#endif //HIRAETHENGINE_TEXTURE_H