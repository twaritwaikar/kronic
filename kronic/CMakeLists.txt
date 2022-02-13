add_executable(kronic 
    main.cpp
)

add_subdirectory(vendor)

target_link_libraries(kronic PRIVATE kronic_engine)
