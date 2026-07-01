cmake_minimum_required(VERSION 3.20)
project(Assignment4 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(Assignment4
        main.cpp
        Line.h
        TextLine.h
        ContactLine.h
        ChecklistLine.h
        Text.h
        Text.cpp
        Cipher.h
        Cipher.cpp
        TextEditor.h
        TextEditor.cpp)

# Кладемо cipher.dll поруч із .exe, щоб LoadLibrary її знаходив.
add_custom_command(TARGET Assignment4 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/cipher.dll"
        "$<TARGET_FILE_DIR:Assignment4>")
