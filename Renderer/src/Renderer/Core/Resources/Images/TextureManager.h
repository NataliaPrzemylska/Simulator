#pragma once
#include "Texture.h"
namespace Renderer {
	class TextureManager {
	public:
		void init() {};
		void cleanUp();
		void LoadTexture(const char* path);
		void TransitionTextureLayout(Texture& texture, const VkFormat& format, const VkImageLayout& oldLayout, const VkImageLayout& newLayout);
		void CopyBufferToImage(Buffer& buffer, Texture& texture);
	private:
		std::vector<Texture> m_Textures;
	};
}