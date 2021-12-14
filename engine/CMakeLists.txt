add_executable(DFGEngine)

target_sources(DFGEngine PRIVATE
	src/main.cpp
)

set_target_properties( DFGEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug/DFGEngine)
set_target_properties( DFGEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release/DFGEngine)


#External includes
target_include_directories(DFGEngine PRIVATE
	Vendor/Glad/include
	Vendor/SDL/include
)

#External links
target_link_libraries(DFGEngine PRIVATE
	glad
	SDL2::SDL2 
	SDL2::SDL2main
)

add_custom_command(TARGET DFGEngine POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:SDL2::SDL2>        
	$<TARGET_FILE_DIR:DFGEngine>
)