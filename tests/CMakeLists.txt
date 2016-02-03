project(TestRunner C CXX)

find_package(Qt5 5.4 COMPONENTS Core Gui Network Sql Widgets Test)
find_package(Boost REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_executable(${PROJECT_NAME}
    ../src/database.cpp
    database_test.cpp
    vdf_test.cpp
    lib/auto_test.hpp
    main.cpp)

include_directories(
    lib
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Sql_INCLUDE_DIRS}
    ${Qt5Test_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

qt5_use_modules(${PROJECT_NAME}
    Widgets
    Core
    Gui
    Network
    Sql
    Test)

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${QT_MAIN})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/appinfo.vdf ${CMAKE_BINARY_DIR}/Bin)

if(WIN32)
    find_program(WINDEPLOY windeployqt PATHS ${Qt5Core_DIR}/../../../bin NO_DEFAULT_PATH)

    if (WINDEPLOY)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                           # Call windeployqt to copy necessary dependencies into the executable path.
                           COMMAND CMD /C "${WINDEPLOY} $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:--release> --no-translations  $<TARGET_FILE:${PROJECT_NAME}> --dir $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins && move $<TARGET_FILE_DIR:${PROJECT_NAME}>\\plugins\\*.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>"
                           )
    else()
        message(WARNING "Unable to find Qt Windows deployment tool at ${Qt5Core_DIR}/../../../bin, no runtime dependencies will be copied.")
    endif()
endif()

add_test(${PROJECT_NAME} ${PROJECT_NAME})

