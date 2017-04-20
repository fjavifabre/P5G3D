#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "Texture.h"

#include <vector>

namespace Taranis
{
	class RenderTarget
	{
		//friend Renderer

	public:
		unsigned int Width;
		unsigned int Height;
		GLenum Type;

		bool HasDepthAndStencil;

	private:
		unsigned int m_ID;
		GLenum m_Target = GL_TEXTURE_2D;

		Texture m_DepthStencil;
		std::vector<Texture> m_ColorAttachments;

	public:

		RenderTarget(unsigned int width, unsigned int height, GLenum type = GL_UNSIGNED_BYTE, unsigned int nColorAttachements = 1, bool depthAndStencil=true);

		Texture* GetDepthStencilTexture();
		Texture* GetColorTexture(unsigned int index);

		void Resize(unsigned int width, unsigned int height);
		void Resize(glm::vec2 size);

		void SetTarget(GLenum target);

	};
}