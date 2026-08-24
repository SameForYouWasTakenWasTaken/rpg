add_subdirectory(Shared)
add_subdirectory(Core)
add_subdirectory(App)

add_executable(Game2 main.cpp)

target_compile_features(Game2 PRIVATE cxx_std_23)
target_include_directories(Game2 PRIVATE .)

target_link_libraries(Game2
        PRIVATE
        App
        SFML::Graphics
        SFML::Window
        SFML::System
        glm::glm
        EnTT::EnTT
)

add_custom_command(TARGET Game2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        App/assets
        $<TARGET_FILE_DIR:Game2>/assets
)

add_custom_command(TARGET Game2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        App/data
        $<TARGET_FILE_DIR:Game2>/data
)