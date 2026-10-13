add_library(engine STATIC
    src/systems/MovementSystem.cpp
    src/systems/BounceSystem.cpp
    src/systems/ParticleSystem.cpp
    src/systems/RenderSystem.cpp
    src/audio/AdaptiveMusic.cpp

    # Implementatie achter de facade, compile-time gekozen met ENGINE_BACKEND
    src/graphics/${ENGINE_BACKEND}/Window.cpp
    src/graphics/${ENGINE_BACKEND}/Renderer.cpp
    src/input/${ENGINE_BACKEND}/Input.cpp
    src/audio/${ENGINE_BACKEND}/AudioFactory.cpp
    src/audio/${ENGINE_BACKEND}/SdlCoreAudioBackend.cpp
    src/audio/${ENGINE_BACKEND}/SdlMixerAudioBackend.cpp
)

target_include_directories(engine PUBLIC src)

# SDL is PRIVATE: code die de engine gebruikt (game/) kan geen SDL-headers includen.
# vcpkg levert op sommige platforms alleen de -static targets (bv. arm64-osx), vandaar de $<IF:...>.
target_link_libraries(engine PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    project_sanitizers
    project_warnings
)

# We hebben een gewone main() en gebruiken SDL2main dus niet.
target_compile_definitions(engine PRIVATE SDL_MAIN_HANDLED)
