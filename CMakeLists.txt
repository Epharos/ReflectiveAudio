cmake_minimum_required(VERSION 3.28...4.2.3)

project(
	RealisticSound
	VERSION 1.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(Vulkan REQUIRED)

#FetchContent_Declare(
#	vk_bootstrap
#	GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
#	GIT_TAG v1.4.341
#)
#FetchContent_MakeAvailable(vk_bootstrap)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

add_subdirectory(Runtime)
