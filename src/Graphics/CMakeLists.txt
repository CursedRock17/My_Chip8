add_library(Graphics GraphicsClass.cpp)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)


target_link_libraries(Graphics
PUBLIC
${OPENGL_LIBRARIES}
glad::glad
glfw
SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static
)

target_include_directories(Graphics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})