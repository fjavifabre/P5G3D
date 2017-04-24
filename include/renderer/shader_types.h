#pragma once

#include <glm\glm.hpp>
#include "shading/texture.h"
#include <string>

namespace Taranis
{
	enum SHADER_TYPE
	{
		SHADER_TYPE_BOOL,
		SHADER_TYPE_INT,
		SHADER_TYPE_FLOAT,
		SHADER_TYPE_SAMPLER1D,
		SHADER_TYPE_SAMPLER2D,
		SHADER_TYPE_SAMPLER3D,
		SHADER_TYPE_SAMPLERCUBE,
		SHADER_TYPE_VEC2,
		SHADER_TYPE_VEC3,
		SHADER_TYPE_VEC4,
		SHADER_TYPE_MAT2,
		SHADER_TYPE_MAT3,
		SHADER_TYPE_MAT4,
	};

	struct Uniform
	{
		SHADER_TYPE Type;
		std::string Name;
		int Size;
		unsigned int Location;
	};

	struct VertexAttribute
	{
		SHADER_TYPE  Type;
		std::string  Name;
		int          Size;
		unsigned int Location;
	};

#if OLD
	struct UniformValue
	{
		SHADER_TYPE Type;

		// TODO maybe too much space? check boost any or static any
			//maybe something like stack based store

		//TODO sruct inheritance?
	};

#else

	class UniformValue
	{
	public:

	private:
		SHADER_TYPE m_Type;
		void * m_buffer;

	public:

		UniformValue(bool value)
		{
			m_Type = SHADER_TYPE_BOOL;
			m_buffer = malloc(sizeof(bool));

			memcpy(m_buffer, &value, sizeof(bool));
		}

		UniformValue(int value)
		{
			m_Type = SHADER_TYPE_INT;
			m_buffer = malloc(sizeof(int));

			memcpy(m_buffer, &value, sizeof(int));
		}

		UniformValue(float value)
		{
			m_Type = SHADER_TYPE_FLOAT;
			m_buffer = malloc(sizeof(float));

			memcpy(m_buffer, &value, sizeof(float));
		}

		UniformValue(glm::vec2 value)
		{
			m_Type = SHADER_TYPE_VEC2;
			m_buffer = malloc(sizeof(glm::vec2));

			memcpy(m_buffer, &value, sizeof(glm::vec2));
		}

		UniformValue(glm::vec3 value)
		{
			m_Type = SHADER_TYPE_VEC3;
			m_buffer = malloc(sizeof(glm::vec3));

			memcpy(m_buffer, &value, sizeof(glm::vec3));
		}

		UniformValue(glm::vec4 value)
		{
			m_Type = SHADER_TYPE_VEC4;
			m_buffer = malloc(sizeof(glm::vec4));

			memcpy(m_buffer, &value, sizeof(glm::vec4));
		}

		UniformValue(glm::mat2 value)
		{
			m_Type = SHADER_TYPE_MAT2;
			m_buffer = malloc(sizeof(glm::mat2));

			memcpy(m_buffer, &value, sizeof(glm::mat2));
		}

		UniformValue(glm::mat3 value)
		{
			m_Type = SHADER_TYPE_MAT3;
			m_buffer = malloc(sizeof(glm::mat3));

			memcpy(m_buffer, &value, sizeof(glm::mat3));
		}

		UniformValue(glm::mat4 value)
		{
			m_Type = SHADER_TYPE_MAT4;
			m_buffer = malloc(sizeof(glm::mat4));

			memcpy(m_buffer, &value, sizeof(glm::mat4));
		}

		//Getters
		bool GetBool()
		{
			return *((bool*)m_buffer);
		}
		int GetInt()
		{
			return *((int*)m_buffer);
		}
		float GetFloat()
		{
			return *((float*)m_buffer);
		}
		glm::vec2 GetVec2()
		{
			return *((glm::vec2*)m_buffer);
		}
		glm::vec3 GetVec3()
		{
			return *((glm::vec3*)m_buffer);
		}
		glm::vec4 GetVec4()
		{
			return *((glm::vec4*)m_buffer);
		}
		glm::mat2 GetMat2()
		{
			return *((glm::mat2*)m_buffer);
		}
		glm::mat3 GetMat3()
		{
			return *((glm::mat3*)m_buffer);
		}
		glm::mat4 GetMat4()
		{
			return *((glm::mat4*)m_buffer);
		}

		SHADER_TYPE Type()
		{
			return m_Type;
		}

		~UniformValue()
		{
			free(m_buffer);
		}
	};

#endif


	struct UniformValueSampler
	{
		SHADER_TYPE  Type;
		unsigned int Unit;
		Texture* Texture;

		UniformValueSampler() {}
	};

}