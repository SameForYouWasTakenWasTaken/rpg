add_library(Shared STATIC
        JsonUtil.cpp
)

set_target_properties(Shared PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Shared PUBLIC .)
target_link_libraries(Shared PUBLIC
        glm::glm
        nlohmann_json::nlohmann_json
)

target_compile_features(Shared PRIVATE cxx_std_23)