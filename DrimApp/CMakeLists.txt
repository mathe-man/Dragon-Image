add_executable(DrimApp
	main.cpp
)

# Link solution libs and SDL3
target_link_libraries(DrimApp 
	DrimLib
	DrimEditor
	DrimDebugger
	SDL3
)

# Add includes for SDL and other libs
target_include_directories(DrimApp PRIVATE
	${SDL3_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}
)

# Add lib folder for linking
target_link_directories(DrimApp PRIVATE
	${SDL3_LIB_DIR}
)