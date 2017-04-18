#include "Texture.h"

#include <stb_image.h>
#include <gl\glew.h>
#include <assert.h>

namespace Taranis
{
	Texture::Texture(std::string fileName)
	{
		unsigned char * image = stbi_load(fileName.c_str(), &m_w, &m_h, NULL, STBI_rgb_alpha);

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

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
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		//Create empty
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_w, m_h, 0, format, type, 0);

		//Poor filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &ID);
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

	void Texture::Bind(int unit)
	{
		if (unit >= 0)
			glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(Target, ID);
	}

	void Texture::Resize(unsigned int width, unsigned int height, unsigned int depth)
	{
		Bind();
		if (Target == GL_TEXTURE_1D)
		{
			glTexImage1D(GL_TEXTURE_1D, 0, InternalFormat, width, 0, Format, Type, 0);
		}
		else if (Target == GL_TEXTURE_2D)
		{
			assert(height > 0);
			glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, Type, 0);
		}
		else if (Target == GL_TEXTURE_3D)
		{
			assert(height > 0 && depth > 0);
			glTexImage3D(GL_TEXTURE_3D, 0, InternalFormat, width, height, depth, 0, Format, Type, 0);
		}
	}
}
