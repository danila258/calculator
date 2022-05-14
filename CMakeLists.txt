cmake_minimum_required(VERSION 3.21)
project(calculator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(calculator main.cpp Calculator.cpp Calculator.h RPN.cpp RPN.h)
target_link_libraries(calculator
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

