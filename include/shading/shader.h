#pragma once

#include "mesh/mesh.h"
#include <list>

#include <gl/glew.h> //Siempre antes que GL
//#include <gl/gl.h>
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>  //Librería matemática
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/shader_types.h"
#include "camera/camera.h"


/* Number of max light allowed in the shaders */
#define MAX_LIGHTS 2

namespace Taranis
{

	//!  Shader class. 
	/*!
	Class used to define a Shader. Contains all functions related with render at GPU and IDs of variables.
	*/
	class Shader
	{

		//! Strcut to store informations related with IDs of lights in this shader.
		struct LightIds {
			int uAmb;  //!< Ambiental component ID.
			int uDiff; //!< Diffuse component ID.
			int uPos; //!< Position ID.
			int uDir; //!< Direction ID.
			int uC; //!< Attenuation ID.
			int uCosCutOff; //!< Cosine Cutoff ID.
			int uSpotExponent; //!< Spot attenuation exponent ID.


		};

	private:

		//Lista de mallas(e internamente objetos) usados en este shader
		std::list<Mesh*> meshes;

		//identificadores de programa y shaders
		unsigned int vshader;
		unsigned int fshader;
		

		//Variables Uniform
		int uModelViewMat;
		int uModelViewProjMat;
		int uNormalMat;
		LightIds lights[MAX_LIGHTS];

		//Texturas Uniform
		int uColorTex;
		int uEmiTex;
		int uSpecTex;
		int uNormTex;

		//Atributos
		int inPos;
		int inColor;
		int inNormal;
		int inTexCoord;
		int inTangent;

		//New storing system --->
		std::vector<Uniform> Uniforms;
		std::vector<VertexAttribute> Attributes;


		//shader loader helper
		static GLuint loadShader(const char *fileName, GLenum type);
		static char* loadStringFromFile(const char *fileName, unsigned int &fileLen);

	public:

		unsigned int ID;

		//! Class constructor.
		/*!
		Shader class constructor.

		\param vertex Path to vertex shader file.
		\param fragment Path to fragment shader file.
		*/
		Shader(const char* vertex, const char* fragment);


		//! Add function.
		/*!
		Adds a Mesh to this shader.

		\param mesh Pointer to the Mesh to add.
		*/
		void addMesh(Mesh *mesh);

		//! Position Getter.
		/*!

		\return position variable ID in this shader.
		*/
		int getInPos();

		//! Color Getter.
		/*!

		\return color variable ID in this shader.
		*/
		int getInColor();

		//! Normal Getter.
		/*!

		\return normal variable ID in this shader.
		*/
		int getInNormal();

		//! TexCoord Getter.
		/*!

		\return texture coordinate variable ID in this shader.
		*/
		int getInTexCoord();

		//! Tangent Getter.
		/*!

		\return tangent variable ID in this shader.
		*/
		int getInTangent();

		//! Shader class destructor.
		/*!
		Frees all memory used by this shader.
		*/

		// Activate the shader
		void Use();

		bool HasUniform(std::string name);

		void Set(std::string location, bool value);
		void Set(std::string location, int value);
		void Set(std::string location, float value);
		void Set(std::string location, glm::vec2 value);
		void Set(std::string location, glm::vec3 value);
		void Set(std::string location, glm::vec4 value);
		void Set(std::string location, glm::mat2 value);
		void Set(std::string location, glm::mat3 value);
		void Set(std::string location, glm::mat4 value);

		~Shader();

		private:

			//TODO use map to store
			int getUniformLocation(std::string name);


	};
}


