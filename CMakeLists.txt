# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.15)

project (CmakeUSB_HID)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lusb")



#include_directories("${PROJECT_SOURCE_DIR}/libusb-1.0")
add_subdirectory (CmakeUSB)
#add_subdirectory("${PROJECT_SOURCE_DIR}/libusb-1.0")
#target_include_directories(CmakeUSB PRIVATE "${PROJECT_SOURCE_DIR}/libusb-1.0/")
#target_link_directories(CmakeUSB PRIVATE "${PROJECT_SOURCE_DIR}/libusb-1.0")
# Включите подпроекты.
add_definitions( -D_CRT_SECURE_NO_WARNINGS )