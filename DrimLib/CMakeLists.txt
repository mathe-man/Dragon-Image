
add_library(DrimLib STATIC
	File.cpp File.h
	DrimFile.cpp DrimFile.h
)

target_include_directories(DrimLib PUBLIC ${CMAKE_SOURCE_DIR})