add_library(Graphics GraphicsClass.cpp)

find_package(SDL2 CONFIG REQUIRED)


target_link_libraries(Graphics
PUBLIC
SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static
)

target_include_directories(Graphics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})