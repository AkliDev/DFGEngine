file(GLOB_RECURSE sandbox_SRC CONFIGURE_DEPENDS 
	src/*.cpp 
	src/*.h
)

add_executable(Sandbox ${sandbox_SRC})

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sandbox_SRC})

set_target_properties( Sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug/Sandbox)
set_target_properties( Sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release/Sandbox)

#External includes
target_include_directories(Sandbox PRIVATE
	
	${CMAKE_SOURCE_DIR}/DFGEngine/src
	${CMAKE_SOURCE_DIR}/DFGEngine/Vendor/spdlog/include
	${CMAKE_SOURCE_DIR}/DFGEngine/Vendor/glm
	${CMAKE_SOURCE_DIR}/DFGEngine/Vendor/Glad/include
)

#External links
target_link_libraries(Sandbox PRIVATE
	DFGEngine
	glad::glad
)

add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_CURRENT_LIST_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_custom_command(TARGET Sandbox POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:SDL2::SDL2>        
	$<TARGET_FILE_DIR:Sandbox>
)

target_compile_definitions(Sandbox PRIVATE $<$<CONFIG:DEBUG>:DFG_DEBUG>)
target_compile_definitions(Sandbox PRIVATE $<$<CONFIG:RELEASE>:DFG_RELEASE>)