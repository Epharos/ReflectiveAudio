#include <iostream>
#include <memory>
#include "frame.hpp"
#include "setup.hpp"
#include "vulkan/vulkan.hpp"
#include "window.hpp"

auto main() -> int
{
	std::cout << "Hello, World!" << std::endl;

	VulkanContext context;
	context.CreateInstance();

	context.CreateDebugMessenger();

	context.CreatePhysicalDevice();
	context.CreateQueues();
	context.CreateDevice();

	context.CreateCommandPool();

	Window window{context, vk::Extent2D{720, 480}};

	context.frameContext = std::make_unique<FrameContext>(context, window);
	FrameContext& frame = *context.frameContext;

	frame.CreateSurface();
	frame.CreateSwapchain();

	while(!window.ShouldClose())
		window.PollEvents();

	return 0;
}
