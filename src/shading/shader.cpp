#include "shading/shader.h"


//#include <windows.h> //TODO: check linux comp

#include <fstream>
#include <exception>
#include <string>
#include <iostream>

namespace Taranis
{
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

		// IMPORTANTE Los identificadores de los atributos ANTES DEL LINKEADO
		glBindAttribLocation(ID, 0, "inPos");
		glBindAttribLocation(ID, 1, "inColor");
		glBindAttribLocation(ID, 2, "inNormal");
		glBindAttribLocation(ID, 3, "inTexCoord");
		glBindAttribLocation(ID, 4, "inTangent");

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

		// DESPUÉS DEL LINKEADO
		uNormalMat = glGetUniformLocation(ID, "normal");
		uModelViewMat = glGetUniformLocation(ID, "modelView");
		uModelViewProjMat = glGetUniformLocation(ID, "modelViewProj");
		//Identificadores de textura
		uColorTex = glGetUniformLocation(ID, "colorTex");
		uEmiTex = glGetUniformLocation(ID, "emiTex");
		uSpecTex = glGetUniformLocation(ID, "specTex");
		uNormTex = glGetUniformLocation(ID, "normalTex");

		//Array de luces
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			std::string number = std::to_string(i);

			lights[i].uAmb = glGetUniformLocation(ID, ("lights[" + number + "].Amb").c_str());

			lights[i].uDiff = glGetUniformLocation(ID, ("lights[" + number + "].Diff").c_str());

			lights[i].uPos = glGetUniformLocation(ID, ("lights[" + number + "].Pos").c_str());

			lights[i].uDir = glGetUniformLocation(ID, ("lights[" + number + "].Dir").c_str());

			lights[i].uC = glGetUniformLocation(ID, ("lights[" + number + "].C").c_str());

			lights[i].uCosCutOff = glGetUniformLocation(ID, ("lights[" + number + "].CosCutOff").c_str());

			lights[i].uSpotExponent = glGetUniformLocation(ID, ("lights[" + number + "].SpotExponent").c_str());

		}


		inPos = glGetAttribLocation(ID, "inPos");
		inColor = glGetAttribLocation(ID, "inColor");
		inNormal = glGetAttribLocation(ID, "inNormal");
		inTexCoord = glGetAttribLocation(ID, "inTexCoord");
		inTangent = glGetAttribLocation(ID, "inTangent");
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

