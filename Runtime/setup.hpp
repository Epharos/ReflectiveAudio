#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <format>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> computeFamily;

	bool IsFullComplete() const
	{
		return IsGraphicsComplete() && IsComputeComplete() && IsPresentComplete();
	}

	bool IsGeneralComplete() const
	{
		return IsGraphicsComplete() && IsPresentComplete();
	}

	bool IsGraphicsComplete() const
	{
		return graphicsFamily.has_value();
	}

	bool IsComputeComplete() const
	{
		return computeFamily.has_value();
	}

	bool IsPresentComplete() const
	{
		return presentFamily.has_value();
	}
};

struct VulkanContext
{
	vk::Instance instance;
	vk::PhysicalDevice physicalDevice;
	QueueFamilyIndices queueFamilyIndices;
	vk::Device device;
	vk::CommandPool commandPool;

	vk::detail::DispatchLoaderDynamic dynamicLoader;
	vk::DebugUtilsMessengerEXT debugMessenger;

	auto CreateInstance() -> void;
	auto CreatePhysicalDevice() -> void;
	auto CreateQueues() -> void;
	auto CreateDevice() -> void;

	auto CreateDebugMessenger() -> void;

	auto CreateCommandPool() -> void;
	
	static VKAPI_ATTR vk::Bool32 VKAPI_PTR DebugLayerCallback(
					vk::DebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
					vk::DebugUtilsMessageTypeFlagsEXT _messageType,
					const vk::DebugUtilsMessengerCallbackDataEXT* _callbackData,
					void* _userData);
};
