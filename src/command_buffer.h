#pragma once

#include "Mesh.h"
#include "Material.h"
#include "render_target.h"

// TODO improve this shit

// At the moment only process command one by one in the given order

namespace Taranis
{
	struct RenderCommand
	{
		glm::mat4 Transform;
		Mesh* Mesh;
		Material* Material;



	};

	class CommandBuffer
	{
	private:


		//std::vector<RenderCommand> m_RenderCommands;

		
		std::vector<RenderCommand> m_DeferredRenderCommands;

		std::vector<RenderCommand> m_DeferredAlphaRenderCommands;
		std::vector<RenderCommand> m_PostProcessingRenderCommands;
		std::map<RenderTarget*, std::vector<RenderCommand>> m_CustomRenderCommands;
		
	public:
		CommandBuffer();
		~CommandBuffer();

		void Push(Mesh* mesh, Material* material, glm::mat4 transform, RenderTarget* target = nullptr);
	
		void Clear();

		//First shader --> then texture bind
		void Sort();

		std::vector<RenderCommand> GetDeferredRenderCommands(); //At the moment not used

		std::vector<RenderCommand> GetDeferredAlphaRenderCommands();

		std::vector<RenderCommand> GetCustomRenderCommands(RenderTarget* target);

		std::vector<RenderCommand> GetPostProcessingRenderCommands();

	};

}

