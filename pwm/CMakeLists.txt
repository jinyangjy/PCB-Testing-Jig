cmake_minimum_required(VERSION 3.21)
project(pwm)

set(CMAKE_CXX_STANDARD 20)

include(FindPkgConfig)
pkg_check_modules(ALLEGRO5 REQUIRED allegro-5)
pkg_check_modules(ALLEGRO5 REQUIRED allegro_primitives-5)

set(IMGUI_DIR imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_library(pwm_lib pwm.cpp)

add_executable(driver driver.cpp)
target_link_libraries(driver pwm_lib)

add_executable(test test.cpp)
target_link_libraries(test pwm_lib)

add_executable(gui gui.cpp gui_driver.cpp)
target_link_directories(gui 
	PRIVATE
	${ALLEGRO5_LIBRARY_DIRS}
	)
target_link_libraries(gui ${ALLEGRO5_LIBRARIES} allegro_main)
target_link_libraries(gui pwm_lib)
target_include_directories(gui 
        PRIVATE
        ${ALLEGRO5_INCLUDE_DIRS}
	)
target_sources(gui
        PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
        )
target_sources(gui
        PRIVATE
        ${IMGUI_DIR}/backends/imgui_impl_allegro5.cpp
        )
