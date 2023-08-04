cmake_minimum_required(VERSION 3.5)
project(Piechart LANGUAGES C)
# Specify the include directories for GD
include_directories(/usr/include)

# Specify the library directories for GD
link_directories(/usr/lib/x86_64-linux-gnu -lm)

add_executable(Piechart main.c)
target_link_libraries (Piechart gd m)
install(TARGETS Piechart
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
