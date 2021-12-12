add_executable(DFGEditor)

target_sources(DFGEditor PRIVATE
	src/main.cpp
)

set_target_properties( DFGEditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/DFGEditor)