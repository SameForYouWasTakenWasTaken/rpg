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
        Factories/Gameplay/Map/MapLoader.cpp
        Factories/Gameplay/Map/MapLoader.cpp
        Map/Tileset.cpp
        Map/TileLayer.cpp
        Map/Tilemap.cpp
        Factories/Application.cpp
        Rendering/SpriteSink.hpp
        Rendering/SpriteSink.cpp
        Rendering/ImGuiSink.cpp
        Layers/Console.cpp
)

target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(App
        PUBLIC
        Shared
        Core
        SFML::Graphics
        SFML::Window
        SFML::System
        tinyxml2::tinyxml2
        ImGui-SFML::ImGui-SFML
        glad_gl_compatibility
)

target_compile_features(App PRIVATE cxx_std_23)