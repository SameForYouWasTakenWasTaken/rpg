add_subdirectory(Shared)
add_subdirectory(Core)
add_subdirectory(App)

add_executable(Game2 ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_include_directories(Game2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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
    ${CMAKE_CURRENT_SOURCE_DIR}/App/assets
    $<TARGET_FILE_DIR:Game2>/assets
)