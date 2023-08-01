cmake_minimum_required(VERSION 3.5)

project(Piechart LANGUAGES C)
find_package(gd)
add_executable(Piechart main.c)
target_link_libraries (Piechart gd)
install(TARGETS Piechart
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
