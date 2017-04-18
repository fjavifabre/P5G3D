#include "command_buffer.h"

#include <algorithm>

namespace Taranis
{
	CommandBuffer::CommandBuffer()
	{

	}

	CommandBuffer::~CommandBuffer()
	{
		Clear();
	}

	void CommandBuffer::Push(Mesh *mesh, Material *material, glm::mat4 transform, RenderTarget *target)
	{
		RenderCommand command = {};
		command.Mesh = mesh;
		command.Transform = transform;
		command.Material = material;


		//if (material->Type == MATERIAL_DEFAULT)
		//{
		//m_DeferredRenderCommands.push_back(command);
		//}

		/*else*/ if (material->Type == MATERIAL_CUSTOM || material->Type == MATERIAL_DEFAULT) //default only cause we dont have deferred yet
		{
			// Added?
			if (m_CustomRenderCommands.find(target) == m_CustomRenderCommands.end())
				m_CustomRenderCommands[target] = std::vector<RenderCommand>();

			m_CustomRenderCommands[target].push_back(command);

		}
		else if (material->Type == MATERIAL_POST_PROCESS)
		{
			m_PostProcessingRenderCommands.push_back(command);
		}
	}

		// TODO Check if all in one short is possible


	bool renderSortDeferred(const RenderCommand &a, const RenderCommand &b)
	{
		return false;
	}

	bool renderSortCustom(const RenderCommand &a, const RenderCommand &b)
	{
		/* NOTE(Joey):
		We want strict weak ordering, which states that if two objects x and y are equivalent
		then both f(x,y) and f(y,x) should be false. Thus when comparing the object to itself
		the comparison should always equal false.
		We also want to do multiple sort comparisons in a single pass, so we encapsulate all
		relevant properties inside an n-tuple with n being equal to the number of sort queries
		we want to do. The tuple < comparison operator initially compares its left-most element
		and then works along the next elements of the tuple until an outcome is clear.
		Another (non C++11) alternative is to write out both the < and > case with the == case
		defaulting to false as in:
		if(a1 < b1)
		return true;
		if(b1 > a1)
		return false;

		if(a2 < b2)
		return true;
		if(b2 > a2)
		return false;
		[...] and so on for each query you want to perform
		return false;
		*/
		return std::make_tuple(a.Material->Blend, a.Material->GetShader()->ID) <
			std::make_tuple(b.Material->Blend, b.Material->GetShader()->ID);
	}

	bool renderSortShader(const RenderCommand &a, const RenderCommand &b)
	{
		return a.Material->GetShader()->ID < b.Material->GetShader()->ID;
	}

	void CommandBuffer::Sort()
	{
		std::sort(m_DeferredRenderCommands.begin(), m_DeferredRenderCommands.end(), renderSortDeferred);
		for (auto rtIt = m_CustomRenderCommands.begin(); rtIt != m_CustomRenderCommands.end(); rtIt++)
		{
			std::sort(rtIt->second.begin(), rtIt->second.end(), renderSortCustom);
		}
	}

	void CommandBuffer::Clear()
	{
		m_DeferredRenderCommands.clear();
		m_CustomRenderCommands.clear();
		m_PostProcessingRenderCommands.clear();
	}

	
	std::vector<RenderCommand> CommandBuffer::GetDeferredRenderCommands()
	{
		return m_DeferredRenderCommands;
	}
	
	std::vector<RenderCommand> CommandBuffer::GetCustomRenderCommands(RenderTarget *target)
	{
		return m_CustomRenderCommands[target];
	}
	
	std::vector<RenderCommand> CommandBuffer::GetDeferredAlphaRenderCommands()
	{
		return m_DeferredAlphaRenderCommands;
	}
	
	std::vector<RenderCommand> CommandBuffer::GetPostProcessingRenderCommands()
	{
		return m_PostProcessingRenderCommands;
	}


}