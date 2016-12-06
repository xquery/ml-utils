cmake_minimum_required(VERSION 2.8.2)

project(rapidjson-download NONE)

include(ExternalProject)
ExternalProject_Add(rapidjson-download
                    GIT_REPOSITORY;https://github.com/miloyip/rapidjson.git;GIT_TAG;master;UPDATE_DISCONNECTED 1
                    SOURCE_DIR          "/Volumes/ml/1_marklogic/projects/ml-util/cmake-build-debug/rapidjson-src"
                    BINARY_DIR          "/Volumes/ml/1_marklogic/projects/ml-util/cmake-build-debug/rapidjson-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
)
