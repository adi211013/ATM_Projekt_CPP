cmake_minimum_required(VERSION 3.31)
project(ATM_PROJEKT)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ATM_PROJEKT source/main.cpp
        source/MainWindow.cpp
        headers/MainWindow.h
        source/Account.cpp
        headers/Account.h
        source/AccountWindow.cpp
        headers/AccountWindow.h)
target_link_libraries(ATM_PROJEKT
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

