// CmakeUSB.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once
#include <stdio.h> // printf
#include <stdint.h>
#ifdef WIN32
//#include <libusb.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#else
#include <unistd.h>
#include <usb.h>
#endif // WIN32
#include <lusb0_usb.h>



#define MSG_SEND (0x01 << 5)
#define MSG_GET (0x01 << 5) | 0x80
#define HELLO_GET  0x35 //'5'
#define HELLO_SEND 0x53 //'S'
#define printf printf2
#define usleep _usleep

void _usleep(unsigned long usec)
{
#ifdef WIN32

	std::this_thread::sleep_for(std::chrono::microseconds(usec));
#else
	usleep(usec);
#endif
}

int __cdecl printf2(const char* format, ...)
{
    char str[1024];

    va_list argptr;
    va_start(argptr, format);
    int ret = vsnprintf(str, sizeof(str), format, argptr);
    va_end(argptr);

    OutputDebugStringA(str);

    return ret;
}



int GetMSG(usb_dev_handle* devHandle, char* msg)
{
	//return libusb_control_transfer(devHandle, MSG_GET, 0x01, 0, 0, msg, 4, 1000);
	return usb_control_msg(devHandle, MSG_GET, 0x01, 0, 0, msg, 1, 1000);
}

int SendMSG(usb_dev_handle* devHandle, unsigned char msg)
{
	//отправляет пустоту(
	//return libusb_control_transfer(devHandle, MSG_SEND, 0x09, 0, 0, &msg, 1, 1000);
	return usb_control_msg(devHandle, MSG_SEND, 0x09, 0, msg, 0, 0, 1000);
}


uint32_t g_GetMaskedValue(uint32_t nValue, uint32_t mask)
{
	nValue = nValue & mask;
	uint32_t bMask = 1;
	while ((mask & bMask) == 0)
	{
		nValue = nValue >> 1;
		mask = mask >> 1;
	}
	return nValue;
}
enum class TUVK : uint8_t 
{
	UNKN = 0b00000000,
	MI28 = 0b00010000,
	KA52 = 0b00100000,
	SU25 = 0b01000000
};
enum class ASP : uint8_t 
{
	UN = 0b0000,  //UNKNOWN
	PU = 0b0001,  //GUN
	C8 = 0b0010,  //C8
	UR = 0b0011,  //UR-A
	FA = 0b0100   //BOMBA
};
// TODO: установите здесь ссылки на дополнительные заголовки, требующиеся для программы.
