#pragma once

#include "setup.hpp"
#include "vulkan/vulkan.hpp"
#include "window.hpp"

struct Swapchain
{
	VulkanContext& context;

	vk::SwapchainKHR swapchain;

	vk::SurfaceCapabilitiesKHR surfaceCapabilities;
	std::vector<vk::SurfaceFormatKHR> surfaceFormats;
	std::vector<vk::PresentModeKHR> presentModes;

	vk::SurfaceFormatKHR selectedSurfaceFormat;
	vk::PresentModeKHR selectedPresentMode;

	vk::Extent2D extent;

	std::vector<vk::Image> images;
	std::vector<vk::ImageView> imageViews;

	Swapchain(VulkanContext& _context);
	~Swapchain();

	auto CreateSwapchain() -> void;
	auto DestroySwapchain() -> void;

	auto SetupImagesAndViews() -> void;

	auto QuerySupport() -> void;

	auto SelectSurfaceFormat(
			vk::Format _preferedFormat = vk::Format::eB8G8R8A8Srgb,
			vk::ColorSpaceKHR _preferedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear
		) -> vk::SurfaceFormatKHR;

	auto SelectPresentMode(
			vk::PresentModeKHR _preferedPresentMode = vk::PresentModeKHR::eMailbox
		) -> vk::PresentModeKHR;

	auto SelectExtent() -> vk::Extent2D;
};

class FrameContext
{
public:
	FrameContext(VulkanContext& _context, Window& window);
	~FrameContext();

	auto CreateSurface() -> void;
	auto CreateSwapchain() -> void;

public:
	VulkanContext& context;
	Window& window;
	vk::SurfaceKHR surface;
	Swapchain swapchain;
};
