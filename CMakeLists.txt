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

add_executable(ATM_PROJEKT WIN32 source/main.cpp
        source/MainWindow.cpp
        headers/MainWindow.h
        source/Account.cpp
        headers/Account.h
        source/AccountWindow.cpp
        headers/AccountWindow.h
        source/KeypadWidget.cpp
        headers/KeypadWidget.h
        source/ATM.cpp
        headers/ATM.h
        source/BankSystem.cpp
        headers/BankSystem.h
        headers/WithdrawResult.h
        headers/LoginResult.h
        source/TransactionLog.cpp
        headers/TransactionLog.h)
target_link_libraries(ATM_PROJEKT
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

