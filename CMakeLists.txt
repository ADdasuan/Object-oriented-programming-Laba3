cmake_minimum_required(VERSION 3.20)
project(ForumProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

if(ENABLE_TSAN)
    message(STATUS "ThreadSanitizer enabled")
    add_compile_options(-fsanitize=thread -fPIE)
    add_link_options(-fsanitize=thread -pie)
    set(ENABLE_COVERAGE OFF)
    message(STATUS "Code coverage disabled because ThreadSanitizer is enabled")
endif()

find_package(nlohmann_json 3.11.0 QUIET)

if(NOT nlohmann_json_FOUND)
    find_path(JSON_INCLUDE_DIR nlohmann/json.hpp
            PATHS
            /usr/include
            /usr/local/include
            ${CMAKE_CURRENT_SOURCE_DIR}/libs
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    if(JSON_INCLUDE_DIR)
        message(STATUS "Found nlohmann/json.hpp at: ${JSON_INCLUDE_DIR}")
        add_library(nlohmann_json INTERFACE IMPORTED)
        set_target_properties(nlohmann_json PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${JSON_INCLUDE_DIR}"
        )
        set(JSON_FOUND TRUE)
    else()
        message(WARNING "nlohmann_json not found. JSON support disabled.")
        set(JSON_FOUND FALSE)
    endif()
else()
    message(STATUS "nlohmann_json found via find_package")
    set(JSON_FOUND TRUE)
endif()

find_package(GTest QUIET)

if(NOT GTest_FOUND)
    find_path(GTEST_INCLUDE_DIR gtest/gtest.h
            PATHS
            /usr/include
            /usr/local/include
    )

    find_library(GTEST_LIBRARY gtest
            PATHS
            /usr/lib
            /usr/local/lib
            /usr/lib/x86_64-linux-gnu
    )

    if(GTEST_INCLUDE_DIR AND GTEST_LIBRARY)
        message(STATUS "Found Google Test at: ${GTEST_INCLUDE_DIR}")
        add_library(GTest::gtest UNKNOWN IMPORTED)
        set_target_properties(GTest::gtest PROPERTIES
                IMPORTED_LOCATION "${GTEST_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIR}"
        )
        set(GTEST_FOUND TRUE)
    else()
        message(WARNING "Google Test not found. Tests will be disabled.")
        set(GTEST_FOUND FALSE)
    endif()
else()
    message(STATUS "Google Test found via find_package")
    set(GTEST_FOUND TRUE)
endif()

find_package(Threads REQUIRED)

if(GTEST_FOUND)
    enable_testing()
endif()

set(ALL_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interface
        ${CMAKE_CURRENT_SOURCE_DIR}/include/strategy
        ${CMAKE_CURRENT_SOURCE_DIR}/include/table
        ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/entity/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/repository/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/service/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/view/include
)

add_library(forum_lib STATIC
        libs/entity/src/DefaultUser.cpp
        libs/entity/src/Moderator.cpp
        libs/entity/src/Troll.cpp
        libs/entity/src/User.cpp

        libs/repository/src/Forum.cpp
        libs/repository/src/Section.cpp
        libs/repository/src/StorageUsers.cpp

        libs/service/src/EmergencyAllModeratorBannedStrategy.cpp
        libs/service/src/ForumDataContainer.cpp
        libs/service/src/ForumDataConvector.cpp
        libs/service/src/ForumService.cpp
        libs/service/src/LoadData.cpp
        libs/service/src/SaveData.cpp
        libs/service/src/SectionService.cpp
        libs/service/src/SimulateService.cpp
        libs/service/src/StatService.cpp
        libs/service/src/UserService.cpp
)

target_include_directories(forum_lib PUBLIC ${ALL_INCLUDE_DIRS})

target_link_libraries(forum_lib PUBLIC Threads::Threads)

if(JSON_FOUND)
    if(TARGET nlohmann_json)
        target_link_libraries(forum_lib PUBLIC nlohmann_json)
    elseif(TARGET nlohmann_json::nlohmann_json)
        target_link_libraries(forum_lib PUBLIC nlohmann_json::nlohmann_json)
    endif()
    message(STATUS "JSON support enabled")
endif()

if(MSVC)
    target_compile_options(forum_lib PRIVATE /W4)
else()
    target_compile_options(forum_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(forum_app
        libs/view/src/main.cpp
        libs/view/src/dialog.cpp
)

target_include_directories(forum_app PRIVATE ${ALL_INCLUDE_DIRS})
target_link_libraries(forum_app PRIVATE forum_lib)

if(GTEST_FOUND)
    add_executable(tests_entity tests/EntityTests.cpp)
    target_include_directories(tests_entity PRIVATE ${ALL_INCLUDE_DIRS})
    target_link_libraries(tests_entity PRIVATE forum_lib GTest::gtest GTest::gtest_main Threads::Threads)
    add_test(NAME EntityTests COMMAND tests_entity)

    add_executable(tests_repository tests/RepositoryTests.cpp)
    target_include_directories(tests_repository PRIVATE ${ALL_INCLUDE_DIRS})
    target_link_libraries(tests_repository PRIVATE forum_lib GTest::gtest GTest::gtest_main Threads::Threads)
    add_test(NAME RepositoryTests COMMAND tests_repository)

    add_executable(tests_service tests/ServiceTests.cpp)
    target_include_directories(tests_service PRIVATE ${ALL_INCLUDE_DIRS})
    target_link_libraries(tests_service PRIVATE forum_lib GTest::gtest GTest::gtest_main Threads::Threads)
    add_test(NAME ServiceTests COMMAND tests_service)

    add_executable(tests_table tests/TableTests.cpp)
    target_include_directories(tests_table PRIVATE ${ALL_INCLUDE_DIRS})
    target_link_libraries(tests_table PRIVATE forum_lib GTest::gtest GTest::gtest_main Threads::Threads)
    add_test(NAME TableTests COMMAND tests_table)

    add_executable(tests_all
            tests/EntityTests.cpp
            tests/RepositoryTests.cpp
            tests/ServiceTests.cpp
            tests/TableTests.cpp
    )
    target_include_directories(tests_all PRIVATE ${ALL_INCLUDE_DIRS})
    target_link_libraries(tests_all PRIVATE forum_lib GTest::gtest GTest::gtest_main Threads::Threads)
    add_test(NAME AllTests COMMAND tests_all)

    message(STATUS "Tests enabled: Google Test found")
else()
    message(STATUS "Tests disabled: Google Test not found")

    add_executable(simple_test tests/EntityTests.cpp)
    target_include_directories(simple_test PRIVATE ${ALL_INCLUDE_DIRS})
    target_link_libraries(simple_test PRIVATE forum_lib)
endif()

message(STATUS "==========================================")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Coverage: ${ENABLE_COVERAGE}")
message(STATUS "JSON support: ${JSON_FOUND}")
message(STATUS "Tests: ${GTEST_FOUND}")
message(STATUS "==========================================")

