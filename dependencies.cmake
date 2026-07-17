include(FetchContent)

function(setup_sfml)
    find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
    if(NOT SFML_FOUND)
        message(STATUS "SFML not found locally; attempting to fetch via FetchContent")
        FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 3.1.0
        )
        FetchContent_MakeAvailable(SFML)
    endif()
endfunction()

function(setup_glm)
    find_package(glm QUIET)
    if(glm_FOUND)
        if(NOT TARGET glm::glm)
            add_library(glm::glm ALIAS glm)
        endif()
    else()
        message(STATUS "GLM not found locally; attempting to fetch via FetchContent")
        FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 1.0.1
        )
        FetchContent_MakeAvailable(glm)
        if(NOT TARGET glm::glm)
            add_library(glm::glm ALIAS glm)
        endif()
    endif()
endfunction()

function(setup_entt)
    find_package(EnTT QUIET)
    if(EnTT_FOUND)
        if(NOT TARGET EnTT::EnTT)
            add_library(EnTT::EnTT ALIAS EnTT)
        endif()
    else()
        message(STATUS "EnTT not found locally; attempting to fetch via FetchContent")
        FetchContent_Declare(
            EnTT
            GIT_REPOSITORY https://github.com/skypjack/entt.git
            GIT_TAG v3.13.2
        )
        FetchContent_MakeAvailable(EnTT)
        if(NOT TARGET EnTT::EnTT)
            add_library(EnTT::EnTT ALIAS EnTT)
        endif()
    endif()
endfunction()

function(setup_nlohmann_json)
    find_package(nlohmann_json QUIET)
    if(nlohmann_json_FOUND)
        if(NOT TARGET nlohmann_json::nlohmann_json)
            add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
        endif()
    else()
        message(STATUS "nlohmann_json not found locally; attempting to fetch via FetchContent")
        FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG v3.12.0
        )
        FetchContent_MakeAvailable(nlohmann_json)
        if(NOT TARGET nlohmann_json::nlohmann_json)
            add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
        endif()
    endif()
endfunction()

setup_sfml()
setup_glm()
setup_entt()
setup_nlohmann_json()
