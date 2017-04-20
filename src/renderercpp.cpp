#include "Renderer.h"

#include "string_id.h"
#include "log.h"

namespace Taranis
{
	Renderer::Renderer() {}

	Renderer::~Renderer()
	{
		//delete m_LightMesh;
		//delete m_LightMaterial;
		//delete m_NDCPlane;

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

		}
	}



}