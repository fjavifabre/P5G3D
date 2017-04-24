#include "renderer/renderer.h"

//#include <GLFW\glfw3.h>

#include "utils/string_id.h"
#include "utils/log.h"

namespace Taranis
{
	Renderer::Renderer() {}

	Renderer::~Renderer()
	{
		//delete m_LightMesh;
		//delete m_LightMaterial;
		delete m_Plane;

		for (auto it = m_DefaultMaterials.begin(); it != m_DefaultMaterials.end(); ++it)
			delete it->second;

		delete m_CustomTarget;

		// Post processing
		delete m_PostProcessTarget1;
		delete m_PostProcessingMaterial1;

		//TODO pbr

		// Materials
		for (unsigned int i = 0; i < m_Materials.size(); i++ )
			delete m_Materials[i];

	}

	void Renderer::Init()
	{
		//TODO

		m_Plane = new Quad();
	}

	void Renderer::SetRenderSize(unsigned int width, unsigned int height)
	{
		m_Size.x = width;
		m_Size.y = height;

		m_CustomTarget->Resize(width, height);
		m_PostProcessTarget1->Resize(width, height);
	}

	void Renderer::SetRenderSize(glm::vec2 size)
	{
		m_Size = size;

		m_CustomTarget->Resize(size);
		m_PostProcessTarget1->Resize(size);
	}

	glm::vec2 Renderer::GetRenderSize()
	{
		return m_Size;
	}

	void Renderer::SetTarget(RenderTarget *renderTarget, GLenum target)
	{
		m_CurrentRenderTargetCustom = renderTarget;
		if (renderTarget)
		{
			if (std::find(m_RenderTargetsCustom.begin(), m_RenderTargetsCustom.end(), renderTarget)
				== m_RenderTargetsCustom.end())
			{
				// add it
				m_RenderTargetsCustom.push_back(renderTarget);
			}
		}

	}

	void Renderer::SetCamera(Camera* camera)
	{
		m_Camera = camera;
	}

	Camera* Renderer::GetCamera()
	{
		return m_Camera;
	}

	Material* Renderer::CreateMaterial(std::string base)
	{
		// Check if exist!
		auto found = m_DefaultMaterials.find(SID(base));

		if (found != m_DefaultMaterials.end())
		{
			Material copy = found->second->Copy();
			Material* mat = new Material(copy);
			
			m_Materials.push_back(mat); //TODO better memory management
			return mat;
		}

		else
		{
			Log::Message("Material of template: " + base + " requested, but template did not exist.", LOG_ERROR);
			return nullptr;
		}
	}

	Material* Renderer::CreateCustomMaterial(Shader* shader)
	{
		Material* mat = new Material(shader);
		mat->Type = MATERIAL_CUSTOM;
		m_Materials.push_back(mat);
		return mat;
	}

	Material* Renderer::CreatePostProcessingMaterial(Shader* shader)
	{
		Material* mat = new Material(shader);
		mat->Type = MATERIAL_POST_PROCESS;
		m_Materials.push_back(mat);
		return mat;
	}

	void Renderer::PushRender(Mesh* mesh, Material* material, glm::mat4 transform)
	{
		RenderTarget* target = getCurrentTarget();
		m_CommandBuffer.Push(mesh, material, transform, target);
	}

	//TODO rendernode

	void Renderer::PushPostProcessor(Material* postProcessor)
	{
		// TODO mesh is quad pre-defined
		m_CommandBuffer.Push(nullptr, postProcessor, glm::mat4());
	}

	void Renderer::PushLight(Light* light)
	{
		m_Lights.push_back(light);
	}

	void Renderer::RenderPushedCommands()
	{
		//TODO switch to deffered rendering
		m_CommandBuffer.Sort();

		//TODO std::vector<RenderCommand> defferedCommands = m_CommandBuffer.GetDeferredRenderCommands();
	
		m_RenderTargetsCustom.push_back(nullptr);

		for (unsigned int targetIndex = 0; targetIndex < m_RenderTargetsCustom.size(); ++targetIndex)
		{
			// Check render target
			RenderTarget* renderTarget = m_RenderTargetsCustom[targetIndex];
			if (renderTarget)
			{
				glViewport(0, 0, renderTarget->Width, renderTarget->Height);
				glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->m_ID);

				if (renderTarget->HasDepthAndStencil)
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				else
					glClear(GL_COLOR_BUFFER_BIT);

				// TODO multiple color targets ?


				/*m_Camera->SetPerspective(m_Camera->FOV,*/
				m_Camera->SetPerspProjection(100,
					(float)renderTarget->Width / (float)renderTarget->Height,
						0.1, 100.0f);
			}

			// Default -> render to custom so we could do postproc
			else
			{
				glViewport(0, 0, m_Size.x, m_Size.y);
				glBindFramebuffer(GL_FRAMEBUFFER, m_CustomTarget->m_ID);
				/*m_Camera->SetPerspective(m_Camera->FOV,*/
				m_Camera->SetPerspProjection(100,
					(float)renderTarget->Width / (float)renderTarget->Height,
					0.1, 100.0f);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}

			std::vector<RenderCommand> renderCommands = m_CommandBuffer.GetCustomRenderCommands(renderTarget);

			for (unsigned int i = 0; i < renderCommands.size(); i++)
			{
				renderCustomCommand(&renderCommands[i], m_Camera);
			}
		}

		// TODO Get combination of deffered


		// POST PROCESSING
		std::vector<RenderCommand> postProcessingCommands = m_CommandBuffer.GetPostProcessingRenderCommands();
		for (unsigned int i = 0; i < postProcessingCommands.size(); i++)
		{
			// Alternate
			bool even = i % 2 == 0;
			Blit(even ? m_CustomTarget : m_PostProcessTarget1,
				even ? m_PostProcessTarget1 : m_CustomTarget,
				postProcessingCommands[i].Material);
			
		}

		// Final blit to default framebuffer (tone mapping gamma correction...)
		Blit(postProcessingCommands.size() % 2 == 0 ? m_CustomTarget : m_PostProcessTarget1,
			nullptr, m_PostProcessingMaterial1);

		// Clear command buffer so next frame start from 0
		// TODO set this if flag is true ?

		m_CommandBuffer.Clear();

		//Clear render state
		m_Lights.clear();
		m_RenderTargetsCustom.clear();

		m_CurrentRenderTargetCustom = nullptr;
	}

	void Renderer::Blit(RenderTarget *src, RenderTarget *dst, Material *material, std::string textureUniformName)
	{
		// If destination
		if (dst)
		{
			glViewport(0, 0, dst->Width, dst->Height);
			glBindFramebuffer(GL_FRAMEBUFFER, dst->m_ID);
			if (dst->HasDepthAndStencil)
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			else
				glClear(GL_COLOR_BUFFER_BIT);
		}

		// Else bind to default
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		}

		// If src is given use its color buffer as input to shader
		if (src)
		{
			material->Set(textureUniformName, src->GetColorTexture(0), 0);
		}

		// Render material to quad -> store inside dst
		RenderCommand command;
		command.Material = material;
		command.Mesh = m_Plane;
		renderCustomCommand(&command, nullptr); // Default camera

		// If we use function properly this is unnecesary
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_Size.x, m_Size.y);
	}

	void Renderer::renderCustomCommand(RenderCommand* command, Camera* camera)
	{
		Material* material = command->Material;
		Mesh* mesh = command->Mesh;

		// Changes only if neccesary
		glDepthFunc(material->DepthCompare);
		if (material->Blend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(material->BlendSrc, material->BlendDst);
		}
		else
			glDisable(GL_BLEND);


		// Default uniforms on shaders?
		material->GetShader()->Use();
		if (camera)
		{
			material->GetShader()->Set("projection", *camera->GetProjection());
			material->GetShader()->Set("view", *camera->GetView());
			material->GetShader()->Set("model", command->Transform);
			material->GetShader()->Set("CamPos", camera->Postion);
		}

		// TODO lights

		// for() ...

		// Samplers
		auto* samplers = material->GetSamplerUniforms();
		for (auto it = samplers->begin(); it != samplers->end(); ++it)
		{
			//if (it->second.Type == SHADER_TYPE_SAMPLERCUBE)
			//	it->second.TextureCube->Bind(it->second.Unit);
			//else
			it->second.Texture->Bind(it->second.Unit);

		}

		// Uniforms
		auto* uniforms = material->GetUniforms();
		for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
		{
			switch (it->second.Type)
			{
			case SHADER_TYPE_BOOL:
				material->GetShader()->Set(it->first, it->second.GetBool());
				break;
			case SHADER_TYPE_INT:
				material->GetShader()->Set(it->first, it->second.GetInt());
				break;
			case SHADER_TYPE_FLOAT:
				material->GetShader()->Set(it->first, it->second.GetFloat());
				break;
			case SHADER_TYPE_VEC2:
				material->GetShader()->Set(it->first, it->second.GetVec2());
				break;
			case SHADER_TYPE_VEC3:
				material->GetShader()->Set(it->first, it->second.GetVec3());
				break;
			case SHADER_TYPE_VEC4:
				material->GetShader()->Set(it->first, it->second.GetVec4());
				break;
			case SHADER_TYPE_MAT2:
				material->GetShader()->Set(it->first, it->second.GetMat2());
				break;
			case SHADER_TYPE_MAT3:
				material->GetShader()->Set(it->first, it->second.GetMat3());
				break;
			case SHADER_TYPE_MAT4:
				material->GetShader()->Set(it->first, it->second.GetMat4());
				break;
			default:
				Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
				break;
			}
		}

		glBindVertexArray(mesh->getVAO());

		// TODO FIX

		//if(mesh->getNumVertex() > 0) // TODO cehck indices
		//	glDrawElements(mesh->Topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES, mesh->size(), GL_UNSIGNED_INT, 0);
		//else
		//	glDrawArrays(mesh->Topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES, 0, mesh->getVertexPos().size());

		glBindVertexArray(0);



	}
}