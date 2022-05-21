add_library(core
    log.cpp
)

target_link_libraries(core spdlog glm)
