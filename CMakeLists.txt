cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(PATCH $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(PATCH 666)
endif()

project(allocator VERSION 0.0.${PATCH})
set(CMAKE_CXX_STANDARD 17)
add_executable(allocator main.cpp)

target_include_directories(allocator PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT masteryilerki@yandex.ru)

include(CPack)