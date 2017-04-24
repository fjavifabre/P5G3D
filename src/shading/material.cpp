#include "shading/material.h"

namespace Taranis
{
	Material::Material()
	{

	}
	
	Material::Material(Shader *shader)
	{
		m_Shader = shader;
	}
	
	Shader* Material::GetShader()
	{
		return m_Shader;
	}
	
	void Material::SetShader(Shader *shader)
	{
		m_Shader = shader;
	}
	
	Material Material::Copy()
	{
		Material copy(m_Shader);

		copy.Color = Color;

		copy.DepthTest = DepthTest;
		copy.DepthWrite = DepthWrite;
		copy.DepthCompare = DepthCompare;

		copy.Cull = Cull;
		copy.CullFace = CullFace;
		copy.CullWindingOrder = CullWindingOrder;

		copy.Blend = Blend;
		copy.BlendSrc = BlendSrc;
		copy.BlendDst = BlendDst;
		copy.BlendEquation = BlendEquation;

		copy.m_Uniforms = m_Uniforms;
		copy.m_SamplerUniforms = m_SamplerUniforms;

		return copy;
	}

	void Material::Set(std::string name, bool value)
	{
		m_Uniforms.emplace(name, value);
	}

	void Material::Set(std::string name, int value)
	{
		m_Uniforms.emplace(name, value);
	}

	void Material::Set(std::string name, float value)
	{
		m_Uniforms.emplace(name, value);
	}

	void Material::Set(std::string name, Texture* value, unsigned int unit)
	{
		
		m_SamplerUniforms[name].Unit = unit;
		m_SamplerUniforms[name].Texture = value;

		switch (value->Target)
		{
		case GL_TEXTURE_1D:
			m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLER1D;
			break;
		case GL_TEXTURE_2D:
			m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLER2D;
			break;
		case GL_TEXTURE_3D:
			m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLER3D;
			break;
			//case GL_TEXTURE_CUBE_MAP:
			//	m_SamplerUniforms[name].Type = SHADER_TYPE_SAMPLERCUBE;
			//	break;


		}

		if (m_Shader)
		{
			m_Shader->Use();
			m_Shader->Set(name, (int)unit);
		}
	}

	void Material::Set(std::string name, glm::vec2 value)
	{
		m_Uniforms.emplace(name, value);
	}

	void Material::Set(std::string name, glm::vec3 value)
	{
		m_Uniforms.emplace(name, value);
	}
	void Material::Set(std::string name, glm::vec4 value)
	{
		m_Uniforms.emplace(name, value);
	}
	void Material::Set(std::string name, glm::mat2 value)
	{
		m_Uniforms.emplace(name, value);
	}
	void Material::Set(std::string name, glm::mat3 value)
	{
		m_Uniforms.emplace(name, value);
	}
	void Material::Set(std::string name, glm::mat4 value)
	{
		m_Uniforms.emplace(name, value);
	}

	std::map<std::string, UniformValue>* Material::GetUniforms()
	{
		return &m_Uniforms;
	}

	std::map<std::string, UniformValueSampler>* Material::GetSamplerUniforms()
	{
		return &m_SamplerUniforms;
	}

	
}