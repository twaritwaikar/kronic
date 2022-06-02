add_library(core "log.cpp" "event.h" "event.cpp")

target_link_libraries(core kronic_engine spdlog glm)
