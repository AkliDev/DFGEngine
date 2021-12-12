add_executable(DFGEngine)

target_sources(DFGEngine PRIVATE
	src/main.cpp
)

set_target_properties( DFGEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/DFGEngine)