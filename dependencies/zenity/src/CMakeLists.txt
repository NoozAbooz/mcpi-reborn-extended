cmake_minimum_required(VERSION 3.13.0)

# Start Project
project(zenity)

# PIC
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Warnings
add_compile_options(-Wall -Wextra -Werror -Wpointer-arith -Wnull-dereference -Wno-unused-parameter -Wno-missing-field-initializers -Wno-sign-compare)
add_link_options(-Wl,--no-undefined)
add_definitions(-D_GNU_SOURCE)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# locale.h
include(CheckIncludeFile)
CHECK_INCLUDE_FILE("locale.h" HAVE_LOCALE_H)
if(HAVE_LOCALE_H)
    add_definitions(-DHAVE_LOCALE_H)
endif()

# GTK+
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET "gtk+-3.0>=3.16.0")
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET "glib-2.0>=2.43.4")
link_libraries(PkgConfig::GTK3 PkgConfig::GLIB)

# Build
add_custom_command(
    OUTPUT ".gresource.c"
    COMMAND glib-compile-resources
    ARGS "--target" "${CMAKE_CURRENT_BINARY_DIR}/.gresource.c" "--generate-source" "--sourcedir" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/.gresource.xml"
    DEPENDS "src/.gresource.xml" "src/zenity.ui"
)
add_executable(zenity src/entry.c src/tree.c src/option.c src/util.c src/main.c "${CMAKE_CURRENT_BINARY_DIR}/.gresource.c")
target_compile_definitions(zenity PRIVATE "-DG_LOG_DOMAIN=\"Zenity\"")

# Install
install(TARGETS zenity DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES src/zenity.ui DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/zenity")
