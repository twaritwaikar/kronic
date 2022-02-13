add_subdirectory(spdlog)

target_include_directories(kronic_engine PUBLIC
    spdlog/include/
)

target_link_libraries(kronic_engine PRIVATE spdlog::spdlog)
