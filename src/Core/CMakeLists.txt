add_library(Core STATIC
    Engine.cpp
    Rendering/Renderer.cpp
    Rendering/Camera.cpp
    Entity.cpp
    Rendering/Window.cpp
    Rendering/Atlas.cpp
    Systems/AssetManager.cpp
    Logger.cpp
)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Core
    PUBLIC
        Shared
        nlohmann_json::nlohmann_json
        SFML::Graphics
        glm::glm
        EnTT::EnTT
)

target_compile_features(Core PRIVATE cxx_std_23)