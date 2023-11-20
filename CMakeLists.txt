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

add_executable(gtest_test gtest_test.cxx config.cxx)
target_link_libraries(gtest_test
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    nlohmann_json::nlohmann_json
)
add_test(NAME gtest_test COMMAND gtest_test)

add_executable(boost_test boost_test.cxx)
target_link_libraries(boost_test Boost::unit_test_framework)
add_test(NAME boost_test COMMAND boost_test)
