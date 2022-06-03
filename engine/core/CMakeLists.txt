add_library(core "log.cpp" "event.h" "event.cpp" "renderer.h")

target_link_libraries(core kronic_engine spdlog glm)
