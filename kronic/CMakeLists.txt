add_executable(kronic 
    main.cpp
    kronic_app.cpp
)

add_subdirectory(platform)

target_link_libraries(kronic PUBLIC kronic_engine platform)
