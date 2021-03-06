#include "Texture.h"

#include <fstream>


Texture::Texture()
{
	m_TextureID = 0;
	m_Width = 0;
	m_Height = 0;
	m_BitDepth = 0;
	m_FileLocation = "";
	m_Filter = GL_LINEAR;
	m_IsSampler = false;
}

Texture::Texture(const char* fileLoc, bool flipVert)
	: Texture()
{
	m_FileLocation = fileLoc;
	m_Filter = GL_LINEAR;

	Load(flipVert);
}

Texture::Texture(const char* fileLoc, bool flipVert, bool isSampler)
	: Texture()
{
	m_FileLocation = fileLoc;
	m_Filter = GL_LINEAR;
	m_IsSampler = isSampler;

	Load(flipVert);
}

Texture::Texture(const char* fileLoc, bool flipVert, GLenum filter)
	: Texture()
{
	m_FileLocation = fileLoc;
	m_Filter = filter;

	Load(flipVert);
}

bool Texture::Load(bool flipVert)
{
	if (m_Buffer)
	{
		printf("Texture '%s' already loaded. Skipping...\n", m_FileLocation);
		return true;
	}

	stbi_set_flip_vertically_on_load(flipVert ? 1 : 0);
	m_Buffer = stbi_load(m_FileLocation, (int*)&m_Width, (int*)&m_Height, &m_BitDepth, 0);
	if (!m_Buffer)
	{
		printf("ERROR: Texture failed to load '%s'\n", m_FileLocation);
		return false;
	}

	GLenum internalFormat = 0;
	GLenum dataFormat = 0;
	if (m_BitDepth == 1)
	{
		internalFormat = GL_RED;
		dataFormat = GL_RED;
	}
	else if (m_BitDepth == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}
	else if (m_BitDepth == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, m_Buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	float fileSize = GetFileSize(m_FileLocation) / (1024.0f * 1024.0f);

	printf("Loading texture '%s' [ ID=%d, size=%.2f MB ]\n", m_FileLocation, m_TextureID, fileSize);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (!m_IsSampler)
		stbi_image_free(m_Buffer);

	return true;
}

int Texture::getRed(int x, int z)
{
	return (int)m_Buffer[((z * m_Width + x) * m_BitDepth) + 0];
}

int Texture::getGreen(int x, int z)
{
	return (int)m_Buffer[((z * m_Width + x) * m_BitDepth) + 1];
}

int Texture::getBlue(int x, int z)
{
	return (int)m_Buffer[((z * m_Width + x) * m_BitDepth) + 2];

}

int Texture::getAlpha(int x, int z)
{
	return (int)m_Buffer[((z * m_Width + x) * m_BitDepth) + 3];
}

void Texture::Bind(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	// printf("Texture: bind texture ID=%d to slot=%d\n", m_TextureID, textureUnit);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

float Texture::GetFileSize(const char* filename)
{
	struct stat stat_buf;
	int rc = stat(filename, &stat_buf);
	return rc == 0 ? (float) stat_buf.st_size : -1.0f;
}

void Texture::Clear()
{
	if (m_IsSampler)
		stbi_image_free(m_Buffer);
	glDeleteTextures(1, &m_TextureID);
	m_TextureID = 0;
	m_Width = 0;
	m_Height = 0;
	m_BitDepth = 0;
	m_FileLocation = "";
}

Texture::~Texture()
{
	Clear();
}
