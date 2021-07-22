cmake_minimum_required(VERSION 3.9)

# Project description
project(
	kbbi-gtk
	VERSION 1.0.0
	DESCRIPTION "KBBI Offline with GTK3"
)

# Check required package & libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT2GTK4 REQUIRED webkit2gtk-4.0)
pkg_check_modules(LIBKBBI REQUIRED libkbbi)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Targets
add_executable(
	${PROJECT_NAME}
	src/main.c
	src/ui.c
)

# Properties
set_target_properties(
	${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
)

# Includes
target_include_directories(
	${PROJECT_NAME}
	PRIVATE src
)

# Link required libraries
target_link_libraries(
	${PROJECT_NAME}
	${GTK3_LIBRARIES}
	${WEBKIT2GTK4_LIBRARIES}
	${LIBKBBI_LIBRARIES}
)

# Include library's public header directory
target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${GTK_INCLUDE_DIRS}
	PUBLIC ${WEBKIT2GTK4_INCLUDE_DIRS}
	PUBLIC ${LIBKBBI_INCLUDE_DIRS}
)

# Add library's extra compiler flags
target_compile_options(
	${PROJECT_NAME}
	PUBLIC ${GTK3_CLFAGS_OTHER}
	PUBLIC ${WEBKIT2GTK4_CLFAGS_OTHER}
	PUBLIC ${LIBKBBI_CLFAGS_OTHER}
)

# CMake setup
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_C_FLAGS "-Wall -Wextra -fvisibility=hidden")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Install the file
install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Expand uninstall script
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY
)

# Add uninstall target
add_custom_target(
	uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
)
