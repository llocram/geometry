set(PROJECT_SOURCES
    main.cpp
)

add_executable(geometry_tests ${PROJECT_SOURCES})

target_include_directories(geometry_tests
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/../src
      ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_ext
)

add_test(geometry_tests geometry_tests)
