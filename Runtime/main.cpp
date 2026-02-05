#include <iostream>
#include "setup.hpp"

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
	return 0;
}
