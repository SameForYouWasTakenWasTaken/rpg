add_library(Shared STATIC 
    dummy.cpp
)

set_target_properties(Shared PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Shared PUBLIC 
    glm::glm
)