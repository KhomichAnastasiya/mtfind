cmake_minimum_required(VERSION 3.16)

project(mtfind LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mtfind main.cpp
    mtfind.h mtfind.cpp

    tests.h
    patternsearcher.h patternsearcher.cpp
    filereader.h filereader.cpp
    output.h output.cpp)

include(GNUInstallDirs)
install(TARGETS mtfind
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
