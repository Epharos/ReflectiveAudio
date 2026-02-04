#include "setup.hpp"
#include <algorithm>

auto VulkanContext::CreateInstance() -> void
{
	vk::ApplicationInfo appInfo("Audio", 0, "Audio", 0, VK_MAKE_API_VERSION(0, 1, 4, 0));
	
	std::vector<const char*> extensions { 
		VK_KHR_SURFACE_EXTENSION_NAME, 
		"VK_KHR_win32_surface",
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME	
	};
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
			return;
		}
	}

	std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };

	auto availableLayers = vk::enumerateInstanceLayerProperties();

	for(const auto& layer : layers)
	{
		bool found = false;

		for(const auto& availableLayer : availableLayers)
		{
			if(strcmp(layer, availableLayer.layerName))
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			std::cerr << std::format("Could not validate layer {}", layer) << std::endl;
			return;
		}
	}


	vk::InstanceCreateInfo instanceInfo({}, &appInfo,
			static_cast<uint32_t>(layers.size()), layers.data(), 
			static_cast<uint32_t>(extensions.size()), extensions.data());
	instance = vk::createInstance(instanceInfo);

	std::cout << "Instance created" << std::endl;
}

auto VulkanContext::CreatePhysicalDevice() -> void
{
	auto physicalDevices = instance.enumeratePhysicalDevices();

	if(physicalDevices.empty())
	{
		std::cerr << "No physical device available" << std::endl;
		return;
	}

	for(const auto& pd : physicalDevices)
	{
		auto properties = pd.getProperties();

		if(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
		{
			physicalDevice = pd;
			std::cout << std::format("Selected {}", std::string(properties.deviceName.data())) << std::endl;
			break;
		}
	}
	
}

auto VulkanContext::CreateQueues() -> void
{
	QueueFamilyIndices indices;

	auto queueFamilies = physicalDevice.getQueueFamilyProperties();

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
		return;
	}

	queueFamilyIndices = indices;

	std::cout << std::format(
				"Selected queue families [G: {}, P: {}, C: {}]", 
				indices.graphicsFamily.value(), 
				indices.presentFamily.value(), 
				indices.computeFamily.value()
			) 
		<< std::endl;

}

auto VulkanContext::CreateDevice() -> void
{
	std::vector<uint32_t> uniqueQueueFamilies = { 
		*queueFamilyIndices.graphicsFamily,
		*queueFamilyIndices.presentFamily,
		*queueFamilyIndices.computeFamily
	};

	std::sort(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end());
	uniqueQueueFamilies.erase(
			std::unique(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end()), 
			uniqueQueueFamilies.end());

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	constexpr float queuePriority = 1.f;

	for(const auto& queueFamily : uniqueQueueFamilies)
	{
		vk::DeviceQueueCreateInfo queueCreateInfo({}, queueFamily, 1, &queuePriority);
		queueCreateInfos.push_back(queueCreateInfo);
	}

	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME }; // No need to add dynamic rendering as its standard since Vulkan 1.3
	std::vector<const char*> deviceLayers = { "VK_LAYER_KHRONOS_validation" };

	vk::PhysicalDeviceVulkan11Features v11features;
	v11features.shaderDrawParameters = VK_TRUE;

	vk::PhysicalDeviceSynchronization2Features sync2Feature;
	sync2Feature.synchronization2 = VK_TRUE;
	sync2Feature.pNext = &v11features;

	vk::PhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeature;
	dynamicRenderingFeature.dynamicRendering = VK_TRUE;
	dynamicRenderingFeature.pNext = &sync2Feature;

	vk::PhysicalDeviceVulkan12Features v12features;
	v12features.shaderOutputLayer = VK_TRUE;
	v12features.pNext = &dynamicRenderingFeature;

	vk::PhysicalDeviceFeatures2 features2;
	features2.features.samplerAnisotropy = VK_TRUE;
	features2.features.tessellationShader = VK_TRUE;
	features2.features.geometryShader = VK_TRUE;
	features2.pNext = &v12features;

	vk::DeviceCreateInfo deviceInfo({}, 
		static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(), 
		static_cast<uint32_t>(deviceLayers.size()), deviceLayers.data(),
		static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
		nullptr, &features2);

	device = physicalDevice.createDevice(deviceInfo);

	if(device == VK_NULL_HANDLE)
	{
		std::cerr << "Could not create device" << std::endl;
		return;
	}

	std::cout << "Created device" << std::endl;
}
