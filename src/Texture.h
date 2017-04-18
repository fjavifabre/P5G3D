#pragma once

#include <string>
#include <map>

//Class to manage textures

//TODO change to not allocating etc


namespace Taranis
{
	class Texture
	{

	public:
		//TODO use getters/setters

		GLenum Target = GL_TEXTURE2D; //Now only 2D

		private:
			int m_w, m_h; //Size
			
			
			Texture(std::string fileName);
			Texture(int w, int h, GLuint internalFormat, GLenum format, GLenum type); //Empty texture

			~Texture();

			static std::map<std::string, std::pair<Texture*, unsigned int>> m_map;

			

		public:
			//load from file

			unsigned int ID; //Texture ID

			static Texture* MakeTexture(std::string name, std::string fileName);
			static Texture* MakeTexture(std::string name, int w, int h, GLuint internalFormat, GLenum format, GLenum type); //Empty texture
			
			static Texture* GetTexture(std::string name); //Texture management
			static void FreeTexture(std::string name);

			void Resize(unsigned int width, unsigned int height = 0, unsigned int depth = 0);
			void Bind(int unit = -1);


	};
}


