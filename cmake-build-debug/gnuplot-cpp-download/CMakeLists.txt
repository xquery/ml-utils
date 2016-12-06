cmake_minimum_required(VERSION 2.8.2)

project(gnuplot-cpp-download NONE)

include(ExternalProject)
ExternalProject_Add(gnuplot-cpp-download
                    GIT_REPOSITORY;https://github.com/orbitcowboy/gnuplot-cpp.git;GIT_TAG;master;UPDATE_DISCONNECTED 1
                    SOURCE_DIR          "/Volumes/ml/1_marklogic/projects/ml-util/cmake-build-debug/gnuplot-cpp-src"
                    BINARY_DIR          "/Volumes/ml/1_marklogic/projects/ml-util/cmake-build-debug/gnuplot-cpp-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
)
