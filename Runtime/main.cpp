#include <iostream>
#include <vector>
#include <vulkan/vulkan.hpp>

struct VulkanContext
{
	vk::Instance instance;
	vk::PhysicalDevice physicalDevice;
};

auto SetupInstanceAndDevices(VulkanContext& _context) -> int
{
	vk::ApplicationInfo appInfo("Audio", 0, "Audio", 0, VK_MAKE_API_VERSION(0, 1, 4, 0));
	
	std::vector<const char*> extensions { VK_KHR_SURFACE_EXTENSION_NAME, "VK_KHR_win32_surface" };
	auto availableExtensions = vk::enumerateInstanceExtensionProperties();


	for(const auto& extension : extensions)
	{
		bool found = false;

		for(const auto& availableExtension : availableExtensions)
		{
			if(strcmp(extension, availableExtension.extensionName))
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			std::cerr << std::format("Could not validate extension {}", extension) << std::endl;
			return -1;
		}
	}

	vk::InstanceCreateInfo instanceInfo({}, &appInfo, 0, VK_NULL_HANDLE, static_cast<uint32_t>(extensions.size()), extensions.data());
	_context.instance = vk::createInstance(instanceInfo);

	std::cout << "Instance created" << std::endl;

	// -------
	
	auto physicalDevices = _context.instance.enumeratePhysicalDevices();

	if(physicalDevices.empty())
	{
		std::cerr << "No physical device available" << std::endl;
		return -1;
	}

	for(const auto& physicalDevice : physicalDevices)
	{
		auto properties = physicalDevice.getProperties();

		if(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
		{
			_context.physicalDevice = physicalDevice;
			std::cout << std::format("Selected {}", std::string(properties.deviceName.data())) << std::endl;
			break;
		}
	}

	return 0;
}

auto main() -> int
{
	std::cout << "Hello, World!" << std::endl;

	VulkanContext context;
	if(SetupInstanceAndDevices(context) != 0) return -1;

	return 0;
}
