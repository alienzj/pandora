cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME_STR pandora)
project(${PROJECT_NAME_STR} VERSION "0.7.0" LANGUAGES C CXX)
configure_file( include/version.h.in ${CMAKE_BINARY_DIR}/include/version.h )

# add a RELEASE_WITH_ASSERTS build type
set(CMAKE_CXX_FLAGS_RELEASE_WITH_ASSERTS "-O3")

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.62.0 COMPONENTS system filesystem iostreams log thread date_time REQUIRED)

# C++11 required
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else ()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_SYSTEM_NO_DEPRECATED -Wall -Wextra")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_OPENMP")
else ()
    find_package(OpenMP 4.0 REQUIRED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

#print CMAKE debug and release flags to know what we are exactly doing
#message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
#message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
#message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

include(${PROJECT_SOURCE_DIR}/ext/gatb.cmake)
set(EXTERNAL_LIBS
        gatbcore
        hdf5
        hdf5_tools)
link_directories(${gatb_binary_dir}/lib)

include(${PROJECT_SOURCE_DIR}/ext/gtest.cmake)

#include directories as SYSTEM includes, thus warnings will be ignored for these
include_directories(SYSTEM
        ${CMAKE_BINARY_DIR}/include
        ${PROJECT_SOURCE_DIR}/cgranges/cpp
        ${gatb_source_dir}/gatb-core/src
        ${gatb_binary_dir}/include
        )

#normal includes: warnings will be reported for these
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${source_dir}/googletest/include
        ${source_dir}/googlemock/include
)

file(GLOB_RECURSE SRC_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*/*.cpp
        ${PROJECT_SOURCE_DIR}/include/*/*.cpp
        ${PROJECT_SOURCE_DIR}/include/*.hpp
        ${PROJECT_SOURCE_DIR}/include/*.h
        ${PROJECT_SOURCE_DIR}/include/*/*.hpp
        ${PROJECT_SOURCE_DIR}/include/*/*.h
        )

add_executable(${PROJECT_NAME} ${SRC_FILES})
add_dependencies(${PROJECT_NAME} gatb gtest)
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_IOSTREAMS_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${EXTERNAL_LIBS}
        ${CMAKE_DL_LIBS}
        ${ZLIB_LIBRARY})



if(BUILD_COMPARE_INDEXES)
    #Build compare_indexes
    set(SRC_FILES_COMPARE_INDEXES ${SRC_FILES})
    list(REMOVE_ITEM SRC_FILES_COMPARE_INDEXES ${PROJECT_SOURCE_DIR}/src/main.cpp)
    add_executable(compare_indexes ${PROJECT_SOURCE_DIR}/scripts/compare_indexes/compare_indexes.cpp ${SRC_FILES_COMPARE_INDEXES})
    add_dependencies(compare_indexes gatb gtest)
    set_target_properties(compare_indexes PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
    target_link_libraries(compare_indexes
            ${CMAKE_THREAD_LIBS_INIT}
            ${Boost_IOSTREAMS_LIBRARY}
            ${Boost_FILESYSTEM_LIBRARY}
            ${Boost_SYSTEM_LIBRARY}
            ${Boost_LOG_LIBRARY}
            ${Boost_THREAD_LIBRARY}
            ${EXTERNAL_LIBS}
            ${CMAKE_DL_LIBS}
            ${ZLIB_LIBRARY})
endif(BUILD_COMPARE_INDEXES)


enable_testing()
add_subdirectory(test)
