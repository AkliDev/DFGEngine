add_executable(DFGEditor)

target_sources(DFGEditor PRIVATE
	src/main.cpp
)

set_target_properties( DFGEditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug/DFGEditor)
set_target_properties( DFGEditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release/DFGEditor)