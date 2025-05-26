
add_library(DrimDebugger STATIC
	Debug.cpp Debug.h
	ImageGenerator.cpp ImageGenerator.h
)

target_link_libraries(DrimDebugger PUBLIC
	DrimLib
	DrimEditor
)
target_include_directories(DrimDebugger PUBLIC ${CMAKE_SOURCE_DIR})