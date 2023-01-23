// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder

#pragma once
#include "Component.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <SDL_image.h>

class VulkanRenderer;


class MaterialComponent: public Component {
	

	public:
		const char* filename;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;

		VulkanRenderer* renderer;

		MaterialComponent(Component* parent_, const char* filename_);
		virtual ~MaterialComponent();
		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float deltaTime_);
		virtual void Render()const;

		void createTextureImage();

		void createTextureImageView();

		void createTextureSampler();

};

