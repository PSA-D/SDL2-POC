cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
        src/core/Engine.cpp
        src/graphics/Window.cpp
        src/graphics/Renderer.cpp
        src/graphics/Window.cpp
        src/graphics/Window.h
        src/graphics/Window.h
        src/graphics/Renderer.cpp
        src/graphics/Renderer.h
)

target_include_directories(engine PUBLIC
        include
        src
)

target_link_libraries(engine PUBLIC
        SDL2::SDL2
        SDL2_mixer::SDL2_mixer
        project_sanitizers
        project_warnings
)

# We gebruiken zelf `auto main() -> int`, dus SDL2main laten we achterwege.
target_compile_definitions(engine PUBLIC SDL_MAIN_HANDLED)

target_compile_features(engine PUBLIC cxx_std_20)