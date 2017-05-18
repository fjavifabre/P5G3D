#include "shading/shader.h"


//#include <windows.h> //TODO: check linux comp

#include <fstream>
#include <exception>
#include <string>
#include <iostream>

namespace Taranis
{

	inline SHADER_TYPE typeConversion(GLenum glType)
	{
		switch (glType)
		{

		case GL_BOOL:
			return SHADER_TYPE_BOOL;

		case GL_UNSIGNED_INT:
		case GL_INT:
			return SHADER_TYPE_INT;

		case GL_FLOAT:
		case GL_DOUBLE:
			return SHADER_TYPE_FLOAT;

		case GL_SAMPLER_1D:
			return SHADER_TYPE_SAMPLER1D;

		case GL_SAMPLER_2D:
			return SHADER_TYPE_SAMPLER2D;

		case GL_SAMPLER_CUBE:
			return SHADER_TYPE_SAMPLERCUBE;

		case GL_FLOAT_VEC2:
		case GL_DOUBLE_VEC2:
			return SHADER_TYPE_VEC2;

		case GL_FLOAT_VEC3:
		case GL_DOUBLE_VEC3:
			return SHADER_TYPE_VEC3;

		case GL_FLOAT_VEC4:
		case GL_DOUBLE_VEC4:
			return SHADER_TYPE_VEC4;


		case GL_FLOAT_MAT2:
		case GL_DOUBLE_MAT2:
			return SHADER_TYPE_MAT2;

		case GL_FLOAT_MAT3:
		case GL_DOUBLE_MAT3:
			return SHADER_TYPE_MAT3;

		case GL_FLOAT_MAT4:
		case GL_DOUBLE_MAT4:
			return SHADER_TYPE_MAT4;
		}

		return SHADER_TYPE_BOOL; //TODO solve
	}

	Shader::Shader(const char* vertex, const char* fragment)
	{
		// Compila los shaders
		vshader = loadShader(vertex, GL_VERTEX_SHADER);
		fshader = loadShader(fragment, GL_FRAGMENT_SHADER);

		// Crear programa
		ID = glCreateProgram();
		// Asignar los shaders
		glAttachShader(ID, vshader);
		glAttachShader(ID, fshader);


		// Linkearlos
		glLinkProgram(ID);

		//Comprobar error
		int linked;
		glGetProgramiv(ID, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			//Calculamos una cadena de error
			GLint logLen;
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetProgramInfoLog(ID, logLen, NULL, logString);
			delete logString;
			glDeleteProgram(ID);
			exit(-1);
		}

		glDeleteShader(vshader);
		glDeleteShader(fshader);

		// Attributes and uniforms
		int nAttributes, nUniforms;
		glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTES, &nAttributes);
		glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &nUniforms);
		Attributes.resize(nAttributes);
		Uniforms.resize(nUniforms);

		char buffer[128];
		for (unsigned int i = 0; i < nAttributes; i++)
		{
			GLenum glType;
			glGetActiveAttrib(ID, i, sizeof(buffer), 0, &Attributes[i].Size, &glType, buffer);
			Attributes[i].Name = std::string(buffer);

			Attributes[i].Type = typeConversion(glType);
			Attributes[i].Location = glGetAttribLocation(ID, buffer);
		}

		for (unsigned int i = 0; i < nUniforms; i++)
		{
			GLenum glType;
			glGetActiveUniform(ID, i, sizeof(buffer), 0, &Uniforms[i].Size, &glType, buffer);
			Uniforms[i].Name = std::string(buffer);

			Uniforms[i].Type = typeConversion(glType);
			Uniforms[i].Location = glGetUniformLocation(ID, buffer);
		}


	}

	int Shader::getInPos()
	{
		return inPos;
	}

	int Shader::getInColor()
	{
		return inColor;
	}


	int Shader::getInNormal()
	{
		return inNormal;
	}
	int Shader::getInTangent()
	{
		return inTangent;
	}

	int Shader::getInTexCoord()
	{
		return inTexCoord;
	}

	
	GLuint Shader::loadShader(const char *fileName, GLenum type)
	{
		unsigned int fileLen;
		char *source = loadStringFromFile(fileName, fileLen);

		//////////////////////////////////////////////
		//Creación y compilación del Shader
		GLuint shader;
		shader = glCreateShader(type);
		glShaderSource(shader, 1, //1: numero de cadenas del fichero
			(const GLchar **)&source, (const GLint *)&fileLen);
		glCompileShader(shader);
		delete source; // Liberar el recurso utilizado para cargar el shader

					   //Comprobamos que se compiló bien
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); // Obtiene si el shader se ha compilado o no
		if (!compiled)
		{
			//Calculamos una cadena de error
			GLint logLen; // Longitud de la cadena de error
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen); // Reservar el espacio en la cadena de error
			char *logString = new char[logLen]; // Crear el string del error
			glGetShaderInfoLog(shader, logLen, NULL, logString); // Obtener el string con el error

			std::cout << "FILENAME: " << fileName << std::endl;
			std::cout << "Error: " << logString << std::endl;

			delete logString;
			glDeleteShader(shader);

			//std::cout << "Press Return to end the program" << std::endl;
			//std::cin.get();
			//exit(-1);

			//Throw exception
			throw std::runtime_error("File not found");

		}


		return shader;
	}

	char * Shader::loadStringFromFile(const char *fileName, unsigned int &fileLen)
	{
		//Se carga el fichero
		std::ifstream file;
		file.open(fileName, std::ios::in);
		if (!file) return 0;

		//Se calcula la longitud del fichero
		file.seekg(0, std::ios::end);
		fileLen = unsigned int(file.tellg());
		file.seekg(std::ios::beg);

		//Se lee el fichero
		char *source = new char[fileLen + 1];

		int i = 0;
		while (file.good())
		{
			source[i] = char(file.get());
			if (!file.eof()) i++;
			else fileLen = i;
		}
		source[fileLen] = '\0';
		file.close();

		return source;
	}

	void Shader::addMesh(Mesh *mesh)
	{
		meshes.push_back(mesh);
	}

	void Shader::Use()
	{
		glUseProgram(ID);
	}

	bool Shader::HasUniform(std::string name)
	{
		for (unsigned int i = 0; i < Uniforms.size(); i++)
			if (Uniforms[i].Name == name) return true;

		return false;
	}

	void Shader::Set(std::string location, int value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniform1i(loc, value);
	}

	void Shader::Set(std::string location, bool value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniform1i(loc, (int)value);
	}

	void Shader::Set(std::string location, float value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniform1f(loc, value);
	}

	void Shader::Set(std::string location, glm::vec2 value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniform2fv(loc, 1,&value[0]);
	}

	void Shader::Set(std::string location, glm::vec3 value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniform3fv(loc, 1, &value[0]);
	}

	void Shader::Set(std::string location, glm::vec4 value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniform4fv(loc, 1, &value[0]);
	}

	void Shader::Set(std::string location, glm::mat2 value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniformMatrix2fv(loc, 1, GL_FALSE, &value[0][0]);
	}

	void Shader::Set(std::string location, glm::mat3 value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
	}

	void Shader::Set(std::string location, glm::mat4 value)
	{
		int loc = getUniformLocation(location);
		if (loc > -1) glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
	}

	int Shader::getUniformLocation(std::string name)
	{
		for (unsigned int i = 0; i < Uniforms.size(); ++i)
		{
			if (Uniforms[i].Name == name)
				return Uniforms[i].Location;
		}
		return -1;
	}


	Shader::~Shader()
	{
		//Liberar shaders y programa
		glDetachShader(ID, vshader);
		glDetachShader(ID, fshader);
		glDeleteShader(vshader);
		glDeleteShader(fshader);
		glDeleteProgram(ID);
	}
}

