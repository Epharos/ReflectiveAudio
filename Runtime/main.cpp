#include <iostream>
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

	Frame frame{context, window};

	frame.CreateSurface();

	while(!window.ShouldClose())
		window.PollEvents();

	return 0;
}
