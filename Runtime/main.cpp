#include <iostream>
#include "setup.hpp"

auto main() -> int
{
	std::cout << "Hello, World!" << std::endl;

	VulkanContext context;
	context.CreateInstance();
	context.CreatePhysicalDevice();
	context.CreateQueues();
	context.CreateDevice();

	return 0;
}
