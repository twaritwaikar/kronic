add_library(os  "file_system.cpp" "file_system.h" "os.h" )

target_link_libraries(os kronic_engine yaml-cpp)
