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
add_subdirectory(DFGEngine/vendor/openal-soft)
add_subdirectory(DFGEngine/vendor/libsndfile)
add_subdirectory(DFGEngine/vendor/Lua)
add_subdirectory(DFGEngine/vendor/rttr)

set_target_properties(glad PROPERTIES FOLDER "Dependencies")
set_target_properties(freetype PROPERTIES FOLDER "Dependencies")

set_target_properties(mpeg_test PROPERTIES FOLDER "Dependencies/libsndfile")
set_target_properties(ogg_opus_test PROPERTIES FOLDER "Dependencies/libsndfile")
set_target_properties(sndfile PROPERTIES FOLDER "Dependencies/libsndfile")

set_target_properties(SDL2 PROPERTIES FOLDER "Dependencies/SDL2")
set_target_properties(SDL2main PROPERTIES FOLDER "Dependencies/SDL2")
set_target_properties(SDL2-static PROPERTIES FOLDER "Dependencies/SDL2")

set_target_properties(OpenAL PROPERTIES FOLDER "Dependencies/OpenAL")
set_target_properties(build_version PROPERTIES FOLDER "Dependencies/OpenAL")
set_target_properties(common PROPERTIES FOLDER "Dependencies/OpenAL")
set_target_properties(ex-common PROPERTIES FOLDER "Dependencies/OpenAL")

set_target_properties(lua_shared PROPERTIES FOLDER "Dependencies/Lua")
set_target_properties(lua_static PROPERTIES FOLDER "Dependencies/Lua")
set_target_properties(luac PROPERTIES FOLDER "Dependencies/Lua")

set_target_properties(rttr_core_lib PROPERTIES FOLDER "Dependencies")

add_subdirectory(DFGEngine)
add_subdirectory(DFGEditor)
add_subdirectory(Sandbox)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)

