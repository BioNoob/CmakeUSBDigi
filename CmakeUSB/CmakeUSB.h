// CmakeUSB.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once
#include <stdio.h> // printf
#include <stdint.h>
#include <unistd.h>
#include <usb.h>
#define MSG_SEND (0x01 << 5)
#define MSG_GET (0x01 << 5) | 0x80
#define HELLO_GET  0x35 //'5'
#define HELLO_SEND 0x53 //'S'
int GetMSG(usb_dev_handle* devHandle, char* msg)
{
	return usb_control_msg(devHandle, MSG_GET, 0x01, 0, 0, msg, 1, 1000);
}

int SendMSG(usb_dev_handle* devHandle, char msg)
{
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
enum TUVK : uint8_t 
{
	UNKN = 0b00000000,
	MI28 = 0b00010000,
	KA52 = 0b00100000,
	SU25 = 0b01000000
};
enum ASP : uint8_t 
{
	UN = 0b0000,  //UNKNOWN
	PU = 0b0001,  //GUN
	C8 = 0b0010,  //C8
	UR = 0b0011,  //UR-A
	FA = 0b0100   //BOMBA
};
// TODO: установите здесь ссылки на дополнительные заголовки, требующиеся для программы.
