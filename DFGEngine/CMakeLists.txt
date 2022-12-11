file(GLOB_RECURSE engine_SRC CONFIGURE_DEPENDS 
	src/*.cpp 
	src/*.h
	vendor/stb_image/**.h,
	vendor/stb_image/**.cpp
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
	Vendor/freetype/include	
	Vendor/Glad/include
	Vendor/glm	
	Vendor/SDL/include
	Vendor/spdlog/include
	Vendor/stb_image
	Vendor/openal-soft/include	
	Vendor/libsndfile/include	
	Vendor/Lua/lua-5.4.4/include	
)

#External links
target_link_libraries(DFGEngine PRIVATE
	freetype
	glad::glad
	SDL2::SDL2 
	SDL2::SDL2main	
	OpenAL::OpenAL
	SndFile::sndfile
	lua_static
	RTTR::Core_Lib
)

target_compile_definitions(DFGEngine PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(DFGEngine PRIVATE $<$<CONFIG:DEBUG>:DFG_DEBUG>)
target_compile_definitions(DFGEngine PRIVATE $<$<CONFIG:RELEASE>:DFG_RELEASE>)