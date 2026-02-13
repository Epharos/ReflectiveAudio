#pragma once

#include "setup.hpp"
#include "vulkan/vulkan.hpp"
#include "window.hpp"

class Frame
{
public:
	Frame(VulkanContext& _context, Window& window);
	~Frame();

	auto CreateSurface() -> void;

private:
	VulkanContext& context;
	Window& window;
	vk::SurfaceKHR surface;

};
