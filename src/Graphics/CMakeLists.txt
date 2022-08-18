add_library(Graphics GraphicsClass.cpp EBO.cpp VAO.cpp VBO.cpp shaderClass.cpp)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(Graphics
PUBLIC
${OPENGL_LIBRARIES}
glad::glad
glfw
)

target_include_directories(Graphics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})