cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 42)
endif()

project(hw07_bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

# Build googletest
exec_program(git ARGS submodule update --init --recursive)
get_filename_component(GTEST_SOURCE ../googletest ABSOLUTE)
add_subdirectory(
        "${GTEST_SOURCE}"
        "googletest"
)

# Boost
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(
        bayan main.cpp
        files.h files.cpp
        hash.h hash.cpp
        store.h
)
add_executable(
        test_bayan
        tests/test_files.cpp
        tests/test_hash.cpp
        tests/test_store.cpp
        files.h files.cpp
        hash.h hash.cpp
        store.h
)

set_target_properties(bayan test_bayan PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

target_include_directories(test_bayan PRIVATE .)

target_link_libraries(test_bayan PRIVATE
        gtest gtest_main
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        )

target_link_libraries(bayan PRIVATE
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        )

if (MSVC)
    target_compile_options(
            bayan PRIVATE
            /W4
    )
    target_compile_options(
            test_bayan PRIVATE
            /W4
    )
else ()
    target_compile_options(
            bayan PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(
            test_bayan PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT j@vladnf.ru)

include(CPack)

enable_testing()

add_test(test_bayan test_bayan)
