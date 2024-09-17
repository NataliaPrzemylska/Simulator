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
		void init(Device& device);
		void cleanUp(const Device& device);
	private:
		void createRenderPass(Device& device);
		void createFrameBuffers(Device& device);
		void createCommandBuffer(Device& device);


		// TODO: move this
		void recordCommandBuffer(Device& device, VkCommandBuffer commandBuffer, uint32_t imageIndex);
	private:
		// TODO: move this
		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CommandBuffer;


		VkPipeline m_NativePipeline;


		VkPipelineLayout m_PipelineLayout;
		VkRenderPass m_RenderPass;

		Shader m_VertexShader;
		Shader m_FragmentShader;
	};
}