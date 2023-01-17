// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder

#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>
#include <vulkan/vulkan.h>


class ShaderComponent: public Component {
	ShaderComponent(const ShaderComponent&) = delete;
	ShaderComponent(ShaderComponent&&) = delete;
	ShaderComponent& operator = (const ShaderComponent&) = delete;
	ShaderComponent& operator = (ShaderComponent&&) = delete;

private:
	const char* vsFilename;
	const char* fsFilename;

public:
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkDescriptorSetLayout descriptorSetLayout;

	ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_);
	~ShaderComponent();
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime ) override;
	void Render() const override;
	void createDescriptorSetLayout();
	void createGraphicsPipeline(const char* vFilename, const char* fFilename);
	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);
};


