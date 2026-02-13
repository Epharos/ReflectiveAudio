#pragma once

#include <GLFW/glfw3.h>
#include "setup.hpp"

struct Window
{
		GLFWwindow* window;

		Window(const VulkanContext& _context, vk::Extent2D _extent);
		~Window();

		auto ShouldClose() -> bool;
		auto PollEvents() -> void;
};
