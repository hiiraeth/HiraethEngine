#include "Texture.h"

#include <iostream>

#include "stb_image.h"
#include "utils.h"

Texture::Texture(GLenum texTarget, std::string &filePath)
{
	m_TextureTarget = texTarget;
	m_FilePath = filePath;
}

Texture::Texture(GLenum texTarget, std::string &filePath, std::string texTag)
{
	m_TextureTarget = texTarget;
	m_FilePath = filePath;
	m_TextureTag = texTag;
}

Texture::Texture() {}

bool Texture::LoadTexture(GLenum texTarget, const std::string &filePath, const std::string &tag)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(texTarget, textureID);

	// Load image data
	int width, height, channels;
	unsigned char* imageData = nullptr;

	if (!LoadSTBIData(filePath, width, height, channels, imageData))
	{
		LOG_ERROR("Failed to load texture from " << filePath << "!\n");
		glDeleteTextures(1, &textureID);
		return false;
	}

	// Determine format based on channels
	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

	// Upload texture data to GPU
	glTexImage2D(texTarget, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(texTarget);

	// Set texture parameters
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind texture
	glBindTexture(texTarget, 0);

	// Free image data
	stbi_image_free(imageData);

	// Store texture ID and tag in collection
	TextureInfo texData;
	texData.ID = textureID;
	texData.tag = tag;

	m_TextureIDs.push_back(texData);
	m_LoadedTextures++;

	LOG_INFO("Loaded texture from " << filePath << " with ID " << textureID << "\n");

	return true;
}

GLuint Texture::LoadTexture()
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(m_TextureTarget, m_TextureID);

	int width, height, channels;
	unsigned char* imageData = nullptr;

	if (!LoadSTBIData(m_FilePath, width, height, channels, imageData))
	{
		LOG_ERROR("Failed to load texture from " << m_FilePath << "!\n");
		return false;
	}

	// Determine format based on channels
	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

	// Upload texture data to GPU
	glTexImage2D(m_TextureTarget, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(m_TextureTarget);

	glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(m_TextureTarget, 0);

	return m_TextureID;
}

unsigned int Texture::LoadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Disable vertical flip for cubemaps
	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			// Determine format based on channel count
			GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			LOG_ERROR("Cubemap tex failed to load at path: " << faces[i]);
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Re-enable vertical flip for standard 2D textures
	stbi_set_flip_vertically_on_load(true);

	return textureID;
}

std::vector<Texture::TextureInfo> Texture::GetTextures()
{
	return m_TextureIDs;
}

bool Texture::LoadSTBIData(const std::string &filePath, int width, int height, int channels, unsigned char* imageData)
{
	stbi_set_flip_vertically_on_load(true);

	imageData = stbi_load(filePath.c_str(), &width,
		&height, &channels,0);

	if (imageData)
	{
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
			0, format, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}

	return true;
}

int Texture::FindTextureID(std::string tag) const
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_LoadedTextures) && (bFound == false))
	{
		if (m_TextureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_TextureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

void Texture::BindGLTextures() const
{
	for (int i = 0; i < m_LoadedTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureIDs[i].ID);
	}
}

