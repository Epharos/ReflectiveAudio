add_executable(Runtime
	"main.cpp"
)

target_include_directories(Runtime
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Runtime
	PUBLIC
		Vulkan::Vulkan
		glfw
)
