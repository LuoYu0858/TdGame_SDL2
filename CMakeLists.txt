cmake_minimum_required(VERSION 4.2)
project(TdGame)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 强制 CMake 重新检测 MSVC 标准库路径
if (MSVC)
    # 设置源文件编码为 UTF-8
    add_compile_options("/utf-8")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES}")
endif ()

# ------ 定义根路径 ------
set(SDL2_ROOT "./ThirdParty/SDL2")
set(SDL2_IMG_ROOT "./ThirdParty/SDL2_image")
set(SDL2_GFX_ROOT "./ThirdParty/SDL2_gfx")
set(SDL2_TTF_ROOT "./ThirdParty/SDL2_ttf")
set(SDL2_MIX_ROOT "./ThirdParty/SDL2_mixer")
set(CJSON_ROOT "./ThirdParty/cJSON")

# ------ 自动收集源文件和头文件 ------
file(GLOB_RECURSE PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# 排除构建目录
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "${CMAKE_BINARY_DIR}.*")

file(GLOB_RECURSE PROJECT_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# 排除构建目录
list(FILTER PROJECT_HEADERS EXCLUDE REGEX "${CMAKE_BINARY_DIR}.*")

# ------ 三方库目录 ------
link_directories(
        ${SDL2_ROOT}/lib/x64
        ${SDL2_IMG_ROOT}/lib/x64
        ${SDL2_GFX_ROOT}/lib/x64
        ${SDL2_TTF_ROOT}/lib/x64
        ${SDL2_MIX_ROOT}/lib/x64
)

# ------ 构建目标 ------
add_executable(TdGame ${PROJECT_SOURCES})

target_sources(TdGame PRIVATE ${CJSON_ROOT}/cJSON.c)

target_include_directories(TdGame PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/manager
        # ------ 三方库头文件 ------
        ${SDL2_ROOT}/include
        ${SDL2_IMG_ROOT}/include
        ${SDL2_GFX_ROOT}/include
        ${SDL2_TTF_ROOT}/include
        ${SDL2_MIX_ROOT}/include
        ${CJSON_ROOT}/include
)

# ------ 链接库 ------
target_link_libraries(TdGame
        SDL2main SDL2
        SDL2_image
        SDL2_gfx
        SDL2_ttf
        SDL2_mixer
)