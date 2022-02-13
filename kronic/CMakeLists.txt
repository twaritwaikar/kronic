add_executable(kronic
    main.cpp
)

target_link_libraries(kronic PRIVATE
    kronic_engine
)
