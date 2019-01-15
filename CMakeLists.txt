cmake_minimum_required(VERSION 3.9)

project(slang C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for shared libraries.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for archive libraries.")

add_subdirectory(3rdparty)

option(ENABLE_GLSLANG_BINARIES "Builds glslangValidator and spirv-remap" OFF)
option(ENABLE_HLSL "Enables HLSL input support" OFF)
add_subdirectory(3rdparty/glslang)

add_subdirectory(3rdparty/SPIRV-Cross)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
add_subdirectory(3rdparty/googletest)

set(SLANG_SRC
    src/video_shader_parse.c
    src/verbosity.c
    src/drivers_shader/slang_preprocess.cpp
    src/drivers_shader/slang_process.cpp
    src/drivers_shader/slang_reflection.cpp
    src/drivers_shader/glslang_util.cpp
    src/glslang/glslang.cpp
    src/SlangShader.m
    )

add_library(slang STATIC ${SLANG_SRC})
target_link_libraries(slang glslang SPIRV common spirv-cross-core spirv-cross-msl)
target_include_directories(slang PUBLIC include)
target_include_directories(slang PRIVATE src)
target_compile_definitions(slang PRIVATE RARCH_INTERNAL)

enable_testing()
add_subdirectory(test)