set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

target_include_directories(kronic_engine PUBLIC ${GLFW_INCLUDE_DIR})

target_link_libraries(kronic_engine PRIVATE glfw)
