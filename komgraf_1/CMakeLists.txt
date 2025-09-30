cmake_minimum_required(VERSION 3.5)

project(komgraf_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Coba cari Qt6 dulu, kalau tidak ada fallback ke Qt5
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_LIB Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    message(STATUS "Using Qt5")
    set(QT_LIB Qt5::Widgets)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    drawingwidget.cpp
    mainwindow.ui
    mainwindow.h
    drawingwidget.h
)

target_link_libraries(${PROJECT_NAME} ${QT_LIB})
