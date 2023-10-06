# CMakeList.txt: проект CMake для CmakeUSB; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project (CmakeUSB)
# Добавьте источник в исполняемый файл этого проекта.
add_executable (CmakeUSB "CmakeUSB.cpp" "CmakeUSB.h")
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
#target_link_libraries(  ${PROJECT_NAME}
						#hidapi_winapi
						#)
#target_link_libraries(CmakeUSB PRIVATE hidapi::hidapi)