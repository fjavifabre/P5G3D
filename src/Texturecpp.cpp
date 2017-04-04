#include "Texture.h"

#include <stb_image.h>
#include <gl\glew.h>

namespace Taranis
{
	Texture::Texture(std::string fileName)
	{
		unsigned char * image = stbi_load(fileName.c_str(), &m_w, &m_h, NULL, STBI_rgb_alpha);

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image);

		delete[] image;

		//Midmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		//Filter
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR); //Filtrado trilinear
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Filtrado lineal
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		
	}

	Texture::Texture(int w, int h, GLuint internalFormat, GLenum format, GLenum type)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		//Create empty
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_w, m_h, 0, format, type, 0);

		//Poor filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
	}

	Texture* Texture::MakeTexture(std::string name, std::string fileName)
	{
		Texture* t = new Texture(fileName);

		m_map.insert(std::make_pair(name,std::make_pair(t, 1)));
	}

	Texture* Texture::MakeTexture(std::string name,int w, int h, GLuint internalFormat, GLenum format, GLenum type)
	{
		Texture* t = new Texture(w, h, internalFormat, format, type);

		m_map.insert(std::make_pair(name, std::make_pair(t, 1)));
	}
	Texture* Texture::GetTexture(std::string name)
	{
		auto it = m_map.find(name);

		if (it != m_map.end()) //Found
		{
			it->second.second = it->second.second + 1;
			return it->second.first;
		}
		else
			return NULL;
			

	}
	void Texture::FreeTexture(std::string name)
	{
		auto it = m_map.find(name);

		if (it != m_map.end()) //Found
		{
			it->second.second = it->second.second - 1;

			if (it->second.second < 1)
			{
				delete it->second.first;
				m_map.erase(it);
			}

		}
		
	}
}
