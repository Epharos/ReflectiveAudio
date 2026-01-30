#include <iostream>
#include <optional>
#include <vulkan/vulkan.hpp>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> computeFamily;

	bool IsFullComplete() const
	{
		return IsGraphicsComplete() && IsComputeComplete() && IsPresentComplete();
	}

	bool IsGeneralComplete() const
	{
		return IsGraphicsComplete() && IsPresentComplete();
	}

	bool IsGraphicsComplete() const
	{
		return graphicsFamily.has_value();
	}

	bool IsComputeComplete() const
	{
		return computeFamily.has_value();
	}

	bool IsPresentComplete() const
	{
		return presentFamily.has_value();
	}
};

struct VulkanContext
{
	vk::Instance instance;
	vk::PhysicalDevice physicalDevice;
	QueueFamilyIndices queueFamilyIndices;
	vk::Device device;
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

	//
	
	QueueFamilyIndices indices;

	auto queueFamilies = _context.physicalDevice.getQueueFamilyProperties();

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			indices.graphicsFamily = i;
			indices.presentFamily = i; // We can assume that graphics family can also present
		}

		if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
		{
			indices.computeFamily = i;
		}

		if (indices.IsFullComplete())
		{
			break;
		}

		i++;
	}

	if(!indices.IsFullComplete())
	{
		std::cerr << "Physical device does not support Graphics, Present and Compute queue" << std::endl;
		return -1;
	}

	_context.queueFamilyIndices = indices;

	std::cout << std::format("Selected queue families [G: {}, P: {}, C: {}]", indices.graphicsFamily.value(), indices.presentFamily.value(), indices.computeFamily.value()) << std::endl;

	return 0;
}

auto main() -> int
{
	std::cout << "Hello, World!" << std::endl;

	VulkanContext context;
	if(SetupInstanceAndDevices(context) != 0) return -1;

	return 0;
}
