add_subdirectory(spdlog)

target_include_directories(kronic_engine PUBLIC spdlog/include)
target_compile_definitions(kronic_engine PUBLIC SPDLOG_COMPILED_LIB=1)

target_link_libraries(kronic_engine PRIVATE spdlog::spdlog)
