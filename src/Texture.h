#pragma once

#include <string>
#include <map>

//Class to manage textures

namespace Taranis
{
	class Texture
	{
		private:
			int m_w, m_h; //Size
			unsigned int m_id; //Texture ID
			
			Texture(std::string fileName);
			Texture(int w, int h, GLuint internalFormat, GLenum format, GLenum type); //Empty texture

			~Texture();

			static std::map<std::string, std::pair<Texture*, unsigned int>> m_map;

			

		public:
			//load from file

			static Texture* MakeTexture(std::string name, std::string fileName);
			static Texture* MakeTexture(std::string name, int w, int h, GLuint internalFormat, GLenum format, GLenum type); //Empty texture
			
			static Texture* GetTexture(std::string name); //Texture management
			static void FreeTexture(std::string name);

	};
}


