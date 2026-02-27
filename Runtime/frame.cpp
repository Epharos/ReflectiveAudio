#include "frame.hpp"
#include "GLFW/glfw3.h"
#include "setup.hpp"
#include "vulkan/vulkan.hpp"
#include "window.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <algorithm>

FrameContext::FrameContext(VulkanContext& _context, Window& _window)
	: context(_context), window(_window), swapchain(_context), surface(VK_NULL_HANDLE)
{

}

FrameContext::~FrameContext()
{
	swapchain.DestroySwapchain();
	if(surface) context.instance.destroySurfaceKHR(surface);
}

auto FrameContext::CreateSurface() -> void
{
	VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;

	if(glfwCreateWindowSurface(context.instance, window.window, nullptr, &surfaceHandle) != VK_SUCCESS)
		throw std::runtime_error("Could not create surface from window");

	surface = surfaceHandle;

	assert(surface && "Surface was not created");
}

auto FrameContext::CreateSwapchain() -> void
{
	swapchain.QuerySupport();
	swapchain.CreateSwapchain();
	swapchain.SetupImagesAndViews();

	std::cout << "Swapchain was created with " << swapchain.images.size() << " images" << std::endl;
}

Swapchain::Swapchain(VulkanContext& _context)
	: context(_context)
{
	
}

Swapchain::~Swapchain()
{
}

auto Swapchain::DestroySwapchain() -> void
{
	if(swapchain) context.device.destroySwapchainKHR(swapchain);
}

auto Swapchain::QuerySupport() -> void
{
	assert(context.frameContext->surface && "Surface cannot be null to query its support");

	surfaceCapabilities = context.physicalDevice.getSurfaceCapabilitiesKHR(context.frameContext->surface);
	surfaceFormats = context.physicalDevice.getSurfaceFormatsKHR(context.frameContext->surface);
	presentModes = context.physicalDevice.getSurfacePresentModesKHR(context.frameContext->surface);

	selectedSurfaceFormat = SelectSurfaceFormat();
	selectedPresentMode = SelectPresentMode();
	extent = SelectExtent();
}

auto Swapchain::SetupImagesAndViews() -> void
{
	images.clear();
	imageViews.clear();

	images = context.device.getSwapchainImagesKHR(swapchain);
	imageViews.resize(images.size());

	int index = 0;

	for(vk::Image& image : images)
	{
		vk::ImageViewCreateInfo info {};
		info.image = image;
		info.format = selectedSurfaceFormat.format;
		info.viewType = vk::ImageViewType::e2D;
		info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.layerCount = 1;

		imageViews[index++] = context.device.createImageView(info);
	}
}

auto Swapchain::SelectSurfaceFormat(
		vk::Format _preferedFormat,
		vk::ColorSpaceKHR _preferedColorSpace
	) -> vk::SurfaceFormatKHR
{
	for(const auto& format : surfaceFormats)
	{
		if(format.format == _preferedFormat && format.colorSpace == _preferedColorSpace)
		{
			return format;
		}
	}

	return surfaceFormats[0];
}

auto Swapchain::SelectPresentMode(
		vk::PresentModeKHR _preferedPresentMode
	) -> vk::PresentModeKHR
{
	for(const auto& presentMode : presentModes)
	{
		if(presentMode == _preferedPresentMode)
		{
			return presentMode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}

auto Swapchain::SelectExtent() -> vk::Extent2D
{
	if(surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return surfaceCapabilities.currentExtent;
	}

	return vk::Extent2D {
		std::clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
		std::clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
	};
}

auto Swapchain::CreateSwapchain() -> void
{
	uint32_t imageCount = std::clamp(2u, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);

	vk::SwapchainCreateInfoKHR info {};

	info.surface = context.frameContext->surface;
	info.minImageCount = imageCount;
	info.clipped = 1;
	info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	info.imageArrayLayers = 1;
	info.imageColorSpace = selectedSurfaceFormat.colorSpace;
	info.imageFormat = selectedSurfaceFormat.format;
	info.imageExtent = extent;
	info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

	const auto& queues = context.queueFamilyIndices;

	uint32_t queueIndices[] = { queues.graphicsFamily.value_or(0), queues.presentFamily.value_or(0) };

	if(queueIndices[0] != queueIndices[1])
	{
		info.imageSharingMode = vk::SharingMode::eConcurrent;
		info.queueFamilyIndexCount = 2;
		info.pQueueFamilyIndices = queueIndices;
	}
	else
	{
		info.imageSharingMode = vk::SharingMode::eExclusive;
	}

	info.preTransform = surfaceCapabilities.currentTransform;
	info.presentMode = selectedPresentMode;
	info.oldSwapchain = nullptr;

	swapchain = context.device.createSwapchainKHR(info);
	
	assert(swapchain && "Swapchain could not be created");
}

