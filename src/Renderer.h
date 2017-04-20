#pragma once

#include "command_buffer.h"

/*
	Main renderer class

	Provides a render buffer and commands to fill it, multiple pass etc.
*/

namespace Taranis
{

	class Renderer
	{
	private:
		CommandBuffer m_CommandBuffer; //Our buffer

		Camera *m_Camera;

		glm::vec2 m_Size; //Render size

		//Lights
		std::vector<Light> m_Lights;

		std::vector<RenderTarget*> m_RenderTargetsCustom; // Custom render targets to use
		RenderTarget* m_CurrentRenderTargetCustom; //Selected render target

		//Quad* plane;

		RenderTarget *m_CustomTarget;

		//Final post-process
		RenderTarget* m_PostProcessTarget1;
		Material* m_PostProcessingMaterial1;

		//Cubemap generation TODO

		//Default materials
		std::map<unsigned int, Material*> m_DefaultMaterials;

		std::vector<Material*> m_Materials; // Generated materials -> Better way?


	public:

		Renderer();
		~Renderer();


		// TODO init func?


		void SetRenderSize(glm::vec2 size);
		void SetRenderSize(unsigned int width, unsigned int height);
		glm::vec2 GetRenderSize();


	};
}