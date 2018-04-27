cmake_minimum_required(VERSION 3.1)

project(MyZipHelper)

set (CMAKE_CXX_STANDARD 14)

# Add minizip lib
find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    PKG_CHECK_MODULES(UNZIP minizip)
endif (PKG_CONFIG_FOUND)
message("UNZIP_LIBRARIES = ${UNZIP_LIBRARIES}")
include_directories(zlib-1.2.7)

set(SRCS
    ./MyZipHelper.h ./MyZipHelper.cpp
    ./zipper.h ./zipper.cpp
    ./unzipper.h ./unzipper.cpp
    )

add_library(${PROJECT_NAME} ${SRCS})

target_link_libraries (${PROJECT_NAME}
    ${UNZIP_LIBRARIES})
