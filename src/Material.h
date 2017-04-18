#pragma once

#include <map>


#include "Shader.h"
#include "Texture.h"
#include "shader_types.h"

namespace Taranis
{
	enum MaterialType
	{
		MATERIAL_DEFAULT,
		MATERIAL_CUSTOM,
		MATERIAL_POST_PROCESS,
	};

	class Material
	{
	private:
		Shader *m_Shader;
		std::map<std::string, UniformValue> m_Uniforms;
		std::map<std::string, UniformValueSampler > m_SamplerUniforms;

	public:

		//--> TODO material type
		MaterialType Type = MATERIAL_CUSTOM;

		glm::vec4 Color = glm::vec4(1.0);

		// Depth
		bool DepthTest = true;
		bool DepthWrite = true;
		GLenum DepthCompare = GL_LESS;

		// Face culling
		bool Cull = true; //true?
		GLenum CullFace = GL_BACK;
		GLenum CullWindingOrder = GL_CCW;

		// Blending state
		bool Blend = false;
		GLenum BlendSrc = GL_SRC_ALPHA;
		GLenum BlendDst = GL_ONE_MINUS_SRC_ALPHA;
		GLenum BlendEquation = GL_FUNC_ADD;

	public:
		Material();
		Material(Shader* shader);

		Shader* GetShader();
		void SetShader(Shader* shader);

		Material Copy();

		void Set(std::string name, bool value);
		void Set(std::string name, int value);
		void Set(std::string name, float value);
		void Set(std::string name, Texture* value, unsigned int unit=0);
		void Set(std::string name, glm::vec2 value);
		void Set(std::string name, glm::vec3 value);
		void Set(std::string name, glm::vec4 value);
		void Set(std::string name, glm::mat2 value);
		void Set(std::string name, glm::mat3 value);
		void Set(std::string name, glm::mat4 value);

		std::map<std::string, UniformValue>* GetUniforms();
		std::map<std::string, UniformValueSampler>* GetSamplerUniforms();
	};
}
