add_library(App STATIC
        Application.cpp
        Scene.cpp
        SceneStack.cpp
        Layers/GameLayer.cpp
        Scenes/GameScene.cpp
        Systems/SpatialGrid.cpp
        Systems/Hierarchy.cpp
        Systems/TransformSystem.cpp
        Factories/Default.cpp
        Systems/Gameplay/Combat.cpp
        Factories/Gameplay/Weapons.cpp
        Factories/Gameplay/Items.cpp
        Systems/Gameplay/InventorySystem.cpp
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

target_compile_features(App PRIVATE cxx_std_23)