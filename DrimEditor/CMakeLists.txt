
add_library(DrimEditor STATIC
	Editor.cpp Editor.h
)

target_link_libraries(DrimEditor PUBLIC
	SDL3
	SDL3_image
)
target_include_directories(DrimEditor PUBLIC ${CMAKE_SOURCE_DIR})