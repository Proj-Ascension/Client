find_package(Qt5Sql REQUIRED)
find_package(Boost REQUIRED)

add_executable(TestRunner
    ../src/database.cpp
    test.cpp)

include_directories(
    lib
    ${Qt5Sql_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

target_link_libraries(TestRunner
    ${Boost_LIBRARIES}
    Qt5::Sql)

add_custom_command(TARGET TestRunner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/appinfo.vdf ${CMAKE_BINARY_DIR})

add_test(TestRunner TestRunner)