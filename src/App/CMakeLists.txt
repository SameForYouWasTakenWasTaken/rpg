add_library(App STATIC
    Application.cpp
    Scene.cpp
    SceneStack.cpp
    Layers/GameLayer.cpp
    Scenes/GameScene.cpp
)

target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(App
    PUBLIC
        Shared
        Core
        SFML::Graphics
        SFML::Window
        SFML::System
)
