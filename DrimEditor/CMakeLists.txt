
add_library(DrimEditor STATIC
	Editor.cpp Editor.h
)

find_package(OpenGL)
target_link_libraries(DrimEditor PUBLIC
	SDL3
	SDL3_image

	imgui
	OpenGL::GL
)
target_include_directories(DrimEditor PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(DrimEditor PUBLIC "${CMAKE_SOURCE_DIR}/extern/imgui")