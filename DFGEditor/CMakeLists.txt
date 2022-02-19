file(GLOB_RECURSE editor_SRC CONFIGURE_DEPENDS 
	src/*.cpp 
	src/*.h
)

add_executable(DFGEditor ${editor_SRC})

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${editor_SRC})

set_target_properties( DFGEditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug/DFGEditor)
set_target_properties( DFGEditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release/DFGEditor)

target_compile_definitions(DFGEditor PRIVATE $<$<CONFIG:DEBUG>:DFG_DEBUG>)
target_compile_definitions(DFGEditor PRIVATE $<$<CONFIG:RELEASE>:DFG_RELEASE>)