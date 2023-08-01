cmake_minimum_required(VERSION 3.5)

# Specify the include directories for GD
include_directories(/usr/include)

# Specify the library directories for GD
link_directories(/usr/lib/x86_64-linux-gnu)

add_executable(Piechart main.c)
target_link_libraries (Piechart gd m)
install(TARGETS Piechart
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
