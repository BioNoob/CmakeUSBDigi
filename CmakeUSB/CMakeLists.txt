# CMakeList.txt: проект CMake для CmakeUSB; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project (CmakeUSB)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CmakeUSB "CmakeUSB.cpp" "CmakeUSB.h")


if(WIN32)
  set(LIBUSB_head lusb0_usb.h)
else()
  set(LIBUSB_head usb.h)
endif()

FIND_PATH(LIBUSB_INCLUDE_DIR NAMES ${LIBUSB_head}
   HINTS
   /usr/local
   ${CMAKE_SOURCE_DIR}/libusb
   PATH_SUFFIXES src
   )
   #   C:/Users/bigja/source/repos/CmakeUSB/MWin32/include WORKED!!!!!
   
if(WIN32)
  set(LIBUSB_NAME libusb0)
else()
  set(LIBUSB_NAME usb)
endif()

 FIND_LIBRARY(LIBUSB_LIBRARY NAMES ${LIBUSB_NAME}
  HINTS
  /usr/local
  ${CMAKE_SOURCE_DIR}/libusb 
  PATH_SUFFIXES lib
)  
message(${LIBUSB_NAME})
message(${LIBUSB_LIBRARY})
message(${LIBUSB_INCLUDE_DIR})
#message(${CMAKE_SOURCE_DIR})
 #C:/Users/bigja/source/repos/CmakeUSB/MWin32/lib WORKED!!!!!
target_include_directories(CmakeUSB PRIVATE ${LIBUSB_INCLUDE_DIR})
target_link_libraries(CmakeUSB  PRIVATE ${LIBUSB_LIBRARY})