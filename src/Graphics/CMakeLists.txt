add_library(Graphics GraphicsInit.cpp)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(Graphics
PUBLIC
glfw
glad::glad
)

target_include_directories(Graphics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})