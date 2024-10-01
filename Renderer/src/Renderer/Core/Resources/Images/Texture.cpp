#include "Texture.h"
#include <stb_image.h>
#include "Application/Application.h"

void Renderer::Texture::LoadTexture(std::string& path){

    Buffer stagingBuffer;
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image! :(");
    }
    
    stagingBuffer = Application::Get()->getResourceManager().createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(Application::Get()->getNativeDevice(), stagingBuffer.m_Memory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(Application::Get()->getNativeDevice(), stagingBuffer.m_Memory);
    stbi_image_free(pixels);

}

void* Renderer::Texture::loadTextureImage(std::string& path)
{
	return nullptr;
}
