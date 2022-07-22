cmake_minimum_required(VERSION 3.22)
project(testing_jig)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Werror")

find_package(PkgConfig)
pkg_check_modules(USB REQUIRED libusb-1.0)  #-L

include_directories(${USB_INCLUDE_DIRS})    # -L
# ^might need to include ${PROJECT_SOURCE_DIR}

link_directories(
        ${USB_LIBRARY_DIRS}  # -L
        )

add_executable(main main.cpp)  # -O
target_link_libraries(main ${USB_LIBRARIES}) #linker


#instead of gcc -o main main.cpp -l libusb 1.0
#we will create a cmake files, to be precise in comparison to a gcc compiler

#<XXX>_LIBRARY_DIRS
#the paths of the libraries (without the '-L')

