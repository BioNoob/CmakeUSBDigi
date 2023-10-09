# CMakeList.txt: проект CMake для CmakeUSB; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project (CmakeUSB)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CmakeUSB "CmakeUSB.cpp" "CmakeUSB.h")



FIND_PATH(LIBUSB_INCLUDE_DIR NAMES libusb.h
   HINTS
   /usr
   /usr/local
   /opt
   ${CMAKE_SOURCE_DIR}/libusb-1.0.26 
   PATH_SUFFIXES libusb
   )
   #   C:/Users/bigja/source/repos/CmakeUSB/MWin32/include WORKED!!!!!
   
if(WIN32)
  set(LIBUSB_NAME libusb-1.0.lib)
else()
  set(LIBUSB_NAME usb-1.0)
endif()

 FIND_LIBRARY(LIBUSB_LIBRARY NAMES ${LIBUSB_NAME}
  HINTS
  /usr
  /usr/local
  /opt
  ${CMAKE_SOURCE_DIR}/libusb-1.0.26 
  PATH_SUFFIXES lib
)  
message(${LIBUSB_NAME})
message(${LIBUSB_LIBRARY})
message(${LIBUSB_INCLUDE_DIR})
message(${CMAKE_SOURCE_DIR})
 #C:/Users/bigja/source/repos/CmakeUSB/MWin32/lib WORKED!!!!!
target_include_directories(CmakeUSB PRIVATE ${LIBUSB_INCLUDE_DIR})
target_link_libraries(CmakeUSB  PRIVATE ${LIBUSB_LIBRARY})