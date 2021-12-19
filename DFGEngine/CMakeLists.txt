file(GLOB_RECURSE engine_SRC CONFIGURE_DEPENDS 
	src/*.cpp 
	src/*.h
)

add_library(DFGEngine STATIC ${engine_SRC})

target_precompile_headers(DFGEngine PRIVATE src/dfgpch.h)

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${engine_SRC})

set_target_properties( DFGEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug/DFGEngine)
set_target_properties( DFGEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release/DFGEngine)

#External includes
target_include_directories(DFGEngine PRIVATE
	src
	Vendor/spdlog/include
	Vendor/SDL/include
	Vendor/Glad/include	
)

#External links
target_link_libraries(DFGEngine PRIVATE
	glad::glad
	SDL2::SDL2 
	SDL2::SDL2main
)