#include "frame.hpp"
#include "GLFW/glfw3.h"
#include "setup.hpp"
#include "window.hpp"
#include <stdexcept>

Frame::Frame(VulkanContext& _context, Window& _window)
	: context(_context), window(_window)
{

}

Frame::~Frame()
{
	if(surface) context.instance.destroySurfaceKHR(surface);
}

auto Frame::CreateSurface() -> void
{
	VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;

	if(glfwCreateWindowSurface(context.instance, window.window, nullptr, &surfaceHandle) != VK_SUCCESS)
		throw std::runtime_error("Could not create surface from window");

	surface = surfaceHandle;
}
