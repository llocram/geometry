set(PROJECT_SOURCES
    main.cpp
)

add_executable(geometry_examples ${PROJECT_SOURCES})

target_include_directories(geometry_examples
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
