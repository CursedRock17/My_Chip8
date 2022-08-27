cmake_minimum_required(VERSION 3.23)
project(Chip8 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME} main.cpp glad.c )

add_subdirectory(src)
add_subdirectory(src/Graphics)
add_subdirectory(src/Shading)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    chip8
    Graphics
    Shading
)


target_include_directories(${PROJECT_NAME}
PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/Graphics"
    "${PROJECT_SOURCE_DIR}/src/Shading"
)