cmake_minimum_required(VERSION 3.20)
project(DirectX11Lab VERSION 0.1.0 LANGUAGES CXX)

if (NOT WIN32)
    message(FATAL_ERROR "DirectX 11 requires Windows.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(directx11_lab WIN32
    src/main.cpp
)

target_include_directories(directx11_lab PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(directx11_lab PRIVATE
    d3d11
    dxgi
)
