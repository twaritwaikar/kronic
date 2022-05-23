add_library(core ../stub.cpp)

add_subdirectory(log)
add_subdirectory(math)
add_subdirectory(renderer)

target_link_libraries(core renderer math log)
