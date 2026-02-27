#include "window.hpp"
#include "GLFW/glfw3.h"
#include "setup.hpp"
#include "vulkan/vulkan.hpp"
#include <iostream>
#include <stdexcept>

Window::Window(const VulkanContext& _context, vk::Extent2D _extent)
{
	if(!glfwInit())
	{
		throw std::runtime_error("Could not initialize glfw");
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(
			_extent.width, 
			_extent.height,
			"Physics Audio",
			nullptr,
			nullptr
		);

	if(!window) std::cout << "Could not create window" << std::endl;
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

auto Window::ShouldClose() -> bool
{
	return glfwWindowShouldClose(window);
}

auto Window::PollEvents() -> void
{
	glfwPollEvents();
}
