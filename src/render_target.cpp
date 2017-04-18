#include "render_target.h"
#include "GL\glew.h"

#include "log.h"

namespace Taranis
{
	RenderTarget::RenderTarget(unsigned int width, unsigned int height, GLenum type, unsigned int nColorAttachments, bool depthAndStencil)
	{
		Height = height;
		Width = width;
		Type = type;

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		// Color attach
		for (unsigned int i = 0; i < nColorAttachments; i++)
		{
			Texture texture;

			// Filtering
			/* 
			texture.FilterMin = GL_LINEAR;
            texture.FilterMax = GL_LINEAR;
            texture.Mipmapping = false;
			
			*/

			//TODO add more

			GLenum internalFormat = GL_RGBA;
			if (type == GL_HALF_FLOAT)
				internalFormat = GL_RGBA16F;
			else if (type == GL_FLOAT)
				internalFormat = GL_RGBA32F;

			//Change 
			texture.Generate(width, height, internalFormat, GL_RGBA, type, 0);
			//Texture::MakeTexture("",width, height, internalFormat, GL_RGBA, type, 0);


			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture.ID, 0);
			m_ColorAttachments.push_back(texture);
		}

		HasDepthAndStencil = depthAndStencil;
		if (depthAndStencil)
		{
			Texture texture;
			//texture.Mipmapping = false;
			texture.Generate(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
		
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture.ID, 0);
			m_DepthStencil = texture;
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Log::Message("Framebuffer not complete!", LOG_ERROR);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	Texture* RenderTarget::GetDepthStencilTexture()
	{
		return &m_DepthStencil;
	}

	Texture* RenderTarget::GetColorTexture(unsigned int index)
	{
		if (index < m_ColorAttachments.size())
			return &m_ColorAttachments[index];
		else
		{
			Log::Message("RenderTarget color texture requested, but not available: " + std::to_string(index), LOG_WARNING);
			return nullptr;
		}
	}

	void RenderTarget::Resize(unsigned int width, unsigned int height)
	{
		Width = width;
		Height = height;

		for (unsigned int i = 0; i < m_ColorAttachments.size(); ++i)
		{
			m_ColorAttachments[i].Resize(width, height);
		}
		// NOTE(Joey): then generate Depth/Stencil texture if requested
		if (HasDepthAndStencil)
		{
			m_DepthStencil.Resize(width, height);
		}
	}
	
	
	void RenderTarget::SetTarget(GLenum target)
	{
		m_Target = target;
	}
}