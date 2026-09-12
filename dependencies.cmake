include(FetchContent)

include(FetchContent)

function(setup_dependency)
    cmake_parse_arguments(
            PARSE_ARGV 0
            ARG
            ""
            "NAME;FIND_NAME;REPO;TAG"
            "TARGETS"
    )

    if (NOT ARG_FIND_NAME)
        set(ARG_FIND_NAME "${ARG_NAME}")
    endif ()

    find_package(${ARG_FIND_NAME} QUIET)

    if (${ARG_FIND_NAME}_FOUND OR ${ARG_NAME}_FOUND)
        message(STATUS "Found local package: ${ARG_FIND_NAME}")
    else ()
        message(STATUS "${ARG_FIND_NAME} not found locally; fetching via FetchContent")
        FetchContent_Declare(
                ${ARG_NAME}
                GIT_REPOSITORY ${ARG_REPO}
                GIT_TAG ${ARG_TAG}
        )
        FetchContent_MakeAvailable(${ARG_NAME})

        # create the directory paths
        string(TOLOWER "${ARG_NAME}" lowercase_name)
        set(${lowercase_name}_SOURCE_DIR "${${lowercase_name}_SOURCE_DIR}" PARENT_SCOPE)
    endif ()

    # Create alias targets ONLY if TARGETS were explicitly provided
    if (ARG_TARGETS)
        foreach (target_pair IN LISTS ARG_TARGETS)
            string(REPLACE ":" ";" pair_list "${target_pair}")
            list(GET pair_list 0 ALIAS_NAME)
            list(GET pair_list -1 ACTUAL_TARGET)

            if (NOT TARGET ${ALIAS_NAME})
                if (TARGET ${ACTUAL_TARGET})
                    add_library(${ALIAS_NAME} ALIAS ${ACTUAL_TARGET})
                elseif (TARGET ${ARG_NAME})
                    add_library(${ALIAS_NAME} ALIAS ${ARG_NAME})
                endif ()
            endif ()
        endforeach ()
    endif ()
endfunction()

setup_dependency(
        NAME SFML
        REPO https://github.com/SFML/SFML.git
        TAG 3.1.0
        TARGETS SFML::Graphics SFML::Window SFML::System
)

setup_dependency(
        NAME glm
        REPO https://github.com/g-truc/glm.git
        TAG 1.0.1
        TARGETS glm::glm
)

setup_dependency(
        NAME EnTT
        REPO https://github.com/skypjack/entt.git
        TAG v3.13.2
        TARGETS EnTT::EnTT
)

setup_dependency(
        NAME nlohmann_json
        REPO https://github.com/nlohmann/json.git
        TAG v3.12.0
        TARGETS nlohmann_json::nlohmann_json
)

setup_dependency(
        NAME tinyxml2
        REPO https://github.com/leethomason/tinyxml2.git
        TAG 11.0.0
        TARGETS tinyxml2::tinyxml2
)

setup_dependency(
        NAME imgui
        REPO https://github.com/ocornut/imgui.git
        TAG v1.91.1
)

# Point ImGui-SFML to the fetched ImGui directory
set(IMGUI_DIR "${imgui_SOURCE_DIR}" CACHE PATH "Path to Dear ImGui" FORCE)
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_SFML_IMGUI_REPO "" CACHE STRING "" FORCE)

setup_dependency(
        NAME ImGui-SFML
        REPO https://github.com/SFML/imgui-sfml.git
        TAG v3.0
        TARGETS ImGui-SFML::ImGui-SFML
)

# GLAD
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v2.0.8
        SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

glad_add_library(
        glad_gl_compatibility
        STATIC
        REPRODUCIBLE
        LOADER
        API gl:compatibility=2.1
)