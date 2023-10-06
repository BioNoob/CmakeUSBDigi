# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.15)

project (CmakeUSB_HID)

#set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lusb")

#add_subdirectory (hidapi)
add_subdirectory (CmakeUSB)



# Включите подпроекты.
add_definitions( -D_CRT_SECURE_NO_WARNINGS )

#target_link_libraries(CmakeUSB PRIVATE hidapi::hidapi)