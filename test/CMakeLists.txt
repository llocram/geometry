set(PROJECT_SOURCES
    main.cpp
)

add_executable(geometry_tests ${PROJECT_SOURCES})

target_include_directories(geometry_tests
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/../include
      ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_ext
)

target_compile_definitions(geometry_tests PRIVATE BOOST_UT_DISABLE_MODULES)

add_test(geometry_tests geometry_tests)
