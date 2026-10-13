add_executable(Game
    src/main.cpp
    src/DemoApp.cpp
    src/SoundGenerator.cpp
    src/effects/BoxesEffect.cpp
    src/effects/ParticleEffect.cpp
)

target_include_directories(Game PRIVATE src)

target_link_libraries(Game PRIVATE
    engine
    project_sanitizers
    project_warnings
)
