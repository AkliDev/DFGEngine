cmake_minimum_required(VERSION 3.22)

# set the project name
project(DFGEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#submodule dependencies
add_subdirectory(DFGEngine/vendor/SDL)
add_subdirectory(DFGEngine/vendor/Glad)
add_subdirectory(DFGEngine/vendor/freetype)

set_target_properties(glad PROPERTIES FOLDER "Dependencies")
set_target_properties(SDL2 PROPERTIES FOLDER "Dependencies")
set_target_properties(SDL2main PROPERTIES FOLDER "Dependencies")
set_target_properties(SDL2-static PROPERTIES FOLDER "Dependencies")
set_target_properties(freetype PROPERTIES FOLDER "Dependencies")

add_subdirectory(DFGEngine)
add_subdirectory(DFGEditor)
add_subdirectory(Sandbox)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

