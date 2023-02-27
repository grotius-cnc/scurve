cmake_minimum_required(VERSION 3.5)

project(scurve LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${SRC_DIR}/libruckig/)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        opengl.h
        opengl.cpp
        formula.h
        sc_struct.h
        sc_engine.h
        sc_engine.cpp
        sc_joints.h
        sc_joints.cpp
        sc_planner.h
        sc_planner.cpp
        sc_arcs.h
        sc_arcs.cpp
        sc_lines.h
        sc_lines.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(motion_tryout
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(motion_tryout SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(motion_tryout
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(motion_tryout PRIVATE Qt${QT_VERSION_MAJOR}::Widgets -lGL pthread rt)
