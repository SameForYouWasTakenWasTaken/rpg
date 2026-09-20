add_subdirectory(Shared)
add_subdirectory(Core)
add_subdirectory(App)

add_executable(Game2 ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_compile_features(Game2 PRIVATE cxx_std_23)
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

function(clone_assets TARGET DIRECTORY DESTINATION)
    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${DIRECTORY}
            $<TARGET_FILE_DIR:Game2>/${DESTINATION}
    )
endfunction()

clone_assets(Game2 ${CMAKE_CURRENT_SOURCE_DIR}/App/assets assets)
clone_assets(Game2 ${CMAKE_CURRENT_SOURCE_DIR}/App/data data)
clone_assets(Game2 ${CMAKE_CURRENT_SOURCE_DIR}/App/scripts scripts)