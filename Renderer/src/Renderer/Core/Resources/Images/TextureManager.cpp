#include "TextureManager.h"
#include "Application/Application.h"

void Renderer::TextureManager::cleanUp()
{
	for (Texture* tex : m_Textures)
	{
		tex->cleanUp();
		delete tex;
	}
}

void Renderer::TextureManager::LoadTexture(const char* path)
{
	Texture* newTexture = new Texture();
	newTexture->LoadTexture(this, path);
	m_Textures.push_back(newTexture);
}

void Renderer::TextureManager::TransitionTextureLayout(Texture& texture, const VkFormat& format, const VkImageLayout& oldLayout, const VkImageLayout& newLayout)
{
	VkCommandBuffer single = Application::Get()->getRenderer().BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;

	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = texture.m_TextureImage;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = 0; // TODO
	barrier.dstAccessMask = 0; // TODO
	vkCmdPipelineBarrier(
		single,
		0 /* TODO */, 0 /* TODO */,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	Application::Get()->getRenderer().EndSingleTimeCommands(single);
}

void Renderer::TextureManager::CopyBufferToImage(Buffer& buffer, Texture& texture)
{
	VkCommandBuffer commandBuffer = Application::Get()->getRenderer().BeginSingleTimeCommands();
	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		texture.width,
		texture.height,
		1
	};
	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer.m_Buffer,
		texture.m_TextureImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);
	Application::Get()->getRenderer().EndSingleTimeCommands(commandBuffer);
}
