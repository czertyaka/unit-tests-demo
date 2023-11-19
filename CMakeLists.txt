cmake_minimum_required(VERSION 3.22)
project(unit-tests-demo CXX)
enable_testing()

find_package(GTest REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(main
    main.cxx
    config.cxx
    database.cxx
)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
configure_file(config.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
