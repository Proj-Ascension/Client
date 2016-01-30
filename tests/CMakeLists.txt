project(TestRunner C CXX)


find_package(Qt5Sql REQUIRED)
find_package(Boost REQUIRED)
add_subdirectory(lib/gtest/googletest)

set(gtest_disable_pthreads on)
set(TEST_FILES
    database_test.cpp
    vdf_test.cpp
    test.cpp)

add_executable(${PROJECT_NAME}
    ../src/database.cpp
    ${TEST_FILES})

include_directories(
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
    ${Qt5Sql_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    gtest
    gtest_main
    ${Boost_LIBRARIES}
    Qt5::Sql)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/appinfo.vdf $<TARGET_FILE_DIR:ProjectAscension>)