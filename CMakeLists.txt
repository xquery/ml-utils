cmake_minimum_required(VERSION 3.2.2)
project(ml_util)

set (ML_UTIL_VERSION_MAJOR 0)
set (ML_UTIL_VERSION_MINOR 2)
set (ML_UTIL_VERSION_PATCH 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -lstdc++ -lpthread -ldl")
set(CTEST_ENVIRONMENT "LC_ALL=C")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(CTest)

# manages deps
include(DownloadProject.cmake)

download_project(PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
        )

download_project(PROJ                rapidjson
        GIT_REPOSITORY      https://github.com/miloyip/rapidjson.git
        GIT_TAG             master
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
        )

download_project(PROJ                gnuplot-cpp
        GIT_REPOSITORY      https://github.com/orbitcowboy/gnuplot-cpp.git
        GIT_TAG             master
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
        )

download_project(PROJ                loguru
        GIT_REPOSITORY      https://github.com/emilk/loguru.git
        GIT_TAG             master
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
        )

#download_project(PROJ                curl
#        GIT_REPOSITORY      https://github.com/curl/curl.git
#        GIT_TAG             master
#        ${UPDATE_DISCONNECTED_IF_AVAILABLE})

# runtime deps
#add_subdirectory(${curl_SOURCE_DIR})

#include_directories(${curl_SOURCE_DIR}/include)
include_directories(${rapidjson_SOURCE_DIR}/include)
include_directories(${gnuplot-cpp_SOURCE_DIR})
include_directories(${loguru_SOURCE_DIR})

# setup testing
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
if(NOT DISABLE_TESTS)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# tests
add_executable(runAllTests test/unit_test_main.cpp)
target_link_libraries(runAllTests gtest gmock_main pthread curl)
add_test( runTests build/runAllTests )
add_executable(runHistTests test/unit_test_history.cpp)
target_link_libraries(runHistTests gtest gmock_main pthread curl)
add_test( runHistTests build/runHistTests )

# build executables
add_executable(ml-hist src/mlhistory/main.cpp)
add_executable(ml-xq src/mlxq/main.cpp)
add_executable(ml-js src/mljs/main.cpp)
add_executable(ml-load src/mlload/main.cpp)
add_executable(ml-status src/mlstatus/main.cpp)
add_executable(ml-log src/mllog/main.cpp)
add_executable(ml-config src/mlconfigure/main.cpp)
add_executable(ml-browse src/mlbrowse/main.cpp)

# link curl
target_link_libraries(ml-hist curl)
target_link_libraries(ml-js curl)
target_link_libraries(ml-xq curl)
target_link_libraries(ml-load curl)
target_link_libraries(ml-status curl)
target_link_libraries(ml-config curl)
target_link_libraries(ml-log curl)
target_link_libraries(ml-browse curl)

# define what to include in release
install(TARGETS ${INS_TARGETS}
        RUNTIME DESTINATION bin COMPONENT applications
        LIBRARY DESTINATION lib COMPONENT libraries
        ARCHIVE DESTINATION lib COMPONENT libraries
        )
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION . COMPONENT applications)
install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION . COMPONENT applications)
install(FILES ${CMAKE_SOURCE_DIR}/VERSION DESTINATION . COMPONENT applications)
install(FILES ${CMAKE_SOURCE_DIR}/etc/.ml-utils DESTINATION . COMPONENT applications)
install(FILES ${CMAKE_SOURCE_DIR}/etc/.ml-utils DESTINATION etc COMPONENT applications)
install(FILES ${CMAKE_SOURCE_DIR}/etc/basic.gnuplot DESTINATION etc COMPONENT applications)
install(FILES ${CMAKE_SOURCE_DIR}/etc/text.gnuplot DESTINATION etc COMPONENT applications)

#install(TARGETS ml-config RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-hist RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-status RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-xq RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-js RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-log RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-load RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-config RUNTIME DESTINATION bin COMPONENT applications)
install(TARGETS ml-browse RUNTIME DESTINATION bin COMPONENT applications)

# cpack defs
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL applications)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MarkLogic commandline utilities")
set(CPACK_PACKAGE_VENDOR "James Fuller")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${ML_UTIL_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${ML_UTIL_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${ML_UTIL_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "RPM")
include(CPack)