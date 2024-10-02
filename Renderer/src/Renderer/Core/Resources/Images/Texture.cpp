#include "Texture.h"
#include <stb_image.h>
#include "Application/Application.h"

void Renderer::Texture::cleanUp()
{
    vkDestroyImage(Application::Get()->getNativeDevice(), m_TextureImage, nullptr);
    vkFreeMemory(Application::Get()->getNativeDevice(), m_TextureImageMemory, nullptr);
}

void Renderer::Texture::LoadTexture(TextureManager* textureManager, const char* path){

    Buffer stagingBuffer;
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    if (!pixels) {
        throw std::runtime_error("failed to load texture image! :(");
    }
    REN_ASSERT(texWidth >= 0 && texHeight >= 0);
    this->width = (uint32_t)texWidth;
    this->height = (uint32_t)texHeight;
    
    stagingBuffer = Application::Get()->getResourceManager().createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(Application::Get()->getNativeDevice(), stagingBuffer.m_Memory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(Application::Get()->getNativeDevice(), stagingBuffer.m_Memory);
    stbi_image_free(pixels);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;
    if (vkCreateImage(Application::Get()->getNativeDevice(), &imageInfo, nullptr, &m_TextureImage) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image! :(");
    }
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Application::Get()->getNativeDevice(), m_TextureImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(Application::Get()->getNativeDevice(), &allocInfo, nullptr, &m_TextureImageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory! :(");
    }

    textureManager->TransitionTextureLayout(*this, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    textureManager->CopyBufferToImage(stagingBuffer, *this);
    textureManager->TransitionTextureLayout(*this, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkBindImageMemory(Application::Get()->getNativeDevice(), m_TextureImage, m_TextureImageMemory, 0);

    

    Application::Get()->getResourceManager().DestroyBuffer(stagingBuffer);
}

