file(GLOB_RECURSE engine_SRC CONFIGURE_DEPENDS 
	src/*.cpp 
	src/*.h
)

add_executable(DFGEngine ${engine_SRC})

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${engine_SRC})

set_target_properties( DFGEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug/DFGEngine)
set_target_properties( DFGEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release/DFGEngine)

#External includes
target_include_directories(DFGEngine PRIVATE
	Vendor/Glad/include
	Vendor/SDL/include
)

#External links
target_link_libraries(DFGEngine PRIVATE
	glad::glad
	SDL2::SDL2 
	SDL2::SDL2main
)

add_custom_command(TARGET DFGEngine POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:SDL2::SDL2>        
	$<TARGET_FILE_DIR:DFGEngine>
)