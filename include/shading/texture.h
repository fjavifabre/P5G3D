#pragma once

#include <GL/glew.h>
#include <string>
#include <map>

//Class to manage textures

//TODO change to not allocating etc


namespace Taranis
{
	class Texture
	{

		public:

			//Use setters and getters
			unsigned int ID; //Texture ID
			GLenum Target = GL_TEXTURE_2D; // type of texture we're dealing with
			GLenum InternalFormat = GL_RGBA; // number of color components
			GLenum Format = GL_RGBA; // format each texel is stored in
			GLenum Type = GL_UNSIGNED_BYTE;
			GLenum FilterMin = GL_LINEAR_MIPMAP_LINEAR; 
			GLenum FilterMax = GL_LINEAR;               
			GLenum WrapS = GL_REPEAT;       
			GLenum WrapT = GL_REPEAT;        
			GLenum WrapR = GL_REPEAT;       
			bool Mipmapping = true;

			unsigned int Width = 0;
			unsigned int Height = 0;
			unsigned int Depth = 0;


			Texture();
			~Texture();

			/* 1D */
			void Generate(unsigned int width, GLenum internalFormat, GLenum format, GLenum type, void *data);
			
			/* 2D */
			void Generate(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, void *data);
			
			/* 2D from file */
			void Generate(std::string fileName);

			/* 3D */
			void Generate(unsigned int width, unsigned int height, unsigned int depth, GLenum internalFormat, GLenum format, GLenum type, void *data);

			void SetWrapMode(GLenum wrapMode, bool bind = false);
			void SetFilterMin(GLenum filter, bool bind = false);
			void SetFilterMax(GLenum filter, bool bind = false);

			void Resize(unsigned int width, unsigned int height = 0, unsigned int depth = 0);
			void Bind(int unit = -1);
			void Unbind();

	};
}


