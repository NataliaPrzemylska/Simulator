#pragma once
#include "Renderer/Core/Shader/Shader.h"
namespace Renderer {
	class Device;
	class SwapChain;
	class Renderer;
	class GraphicsPipeline
	{
	public:
		friend class SwapChain;
		friend class Renderer;
		void init();
		void cleanUp();
		VkPipeline& getNativePipeline() { return m_NativePipeline; };
	private:
		VkPipeline m_NativePipeline;
		VkPipelineLayout m_PipelineLayout;

		Shader m_VertexShader;
		Shader m_FragmentShader;
	};
}