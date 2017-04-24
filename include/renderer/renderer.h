#pragma once

#include "command_buffer.h"
#include "mesh/quad.h"
#include "camera/camera.h"

//#include "light/light.h"
class Light;

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
		std::vector<Light*> m_Lights;

		std::vector<RenderTarget*> m_RenderTargetsCustom; // Custom render targets to use
		RenderTarget* m_CurrentRenderTargetCustom; //Selected render target

		Quad* m_Plane;

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
		void Init();

		void SetRenderSize(glm::vec2 size);
		void SetRenderSize(unsigned int width, unsigned int height);
		glm::vec2 GetRenderSize();

		void SetTarget(RenderTarget* renderTarget, GLenum target = GL_TEXTURE_2D);

		void SetCamera(Camera* camera);
		Camera* GetCamera();

		Material* CreateMaterial(std::string base = "default");
		Material* CreateCustomMaterial(Shader* shader);
		Material* CreatePostProcessingMaterial(Shader* shader);

		void PushRender(Mesh* mesh, Material* material, glm::mat4 transform = glm::mat4());
		//void PushRender(SceneNode* node);
		void PushPostProcessor(Material* postProcessor);

		void PushLight(Light* light);

		void RenderPushedCommands();

		void Blit(RenderTarget *src, RenderTarget *dst, Material *material, std::string textureUniformName = "TexSrc");

		//TODO more methods


	private:
		void renderCustomCommand(RenderCommand* command, Camera* camera);
		RenderTarget* getCurrentTarget();


	};
}