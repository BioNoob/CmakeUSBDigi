#include "CmakeUSB.h"
#include <iostream>
bool was_open;
bool was_heandshaked;
int TryWakeUp(usb_dev_handle*& outdevHandle)
{
	bool was_found = false;
	struct usb_bus* bus = NULL;
	int numInterfaces = 0;
	struct usb_device* device = NULL;
	// Enumerate the USB device tree
	usb_find_busses();
	usb_find_devices();
	bool _breaker = false;
	// Iterate through attached busses and devices
	bus = usb_get_busses();
	while (bus != NULL)
	{
		if (_breaker)
			break;
		device = bus->devices;
		while (device != NULL)
		{
			// Check to see if each USB device matches the DigiSpark Vendor and Product IDs
			if ((device->descriptor.idVendor == 0x16c0) && (device->descriptor.idProduct == 0x05df))
			{
				was_found = true;
				printf("Detected DigiSpark... \n");
				if (!was_open)
				{
					struct usb_dev_handle* devHandle = NULL;
					devHandle = usb_open(device);
					if (devHandle != NULL)
					{
						was_open = true;
						outdevHandle = devHandle;
					}
					else
						was_open = false;
				}
				_breaker = true;
				break;
			}
			device = device->next;
		}
		bus = bus->next;
	}
	free(bus);
	if (!was_found && was_open)
	{
		usb_close(outdevHandle);
		was_open = false;
		was_heandshaked = false;
		return 0;
	}
	if (was_found && was_open)
		return 1;
	else
		return 0;
}
void Worker(usb_dev_handle* devHandle)
{
	int res = 0;
	uint8_t heandshake_r = 0x0;
	uint8_t heandshake_s = HELLO_SEND;
	uint8_t data_r = 0x0;
	uint8_t data_s = 0x0;
	res = -1;
	int i = 0;
	TUVK tvk = TUVK::UNKN;
	ASP asp = ASP::UN;
	//TEST2
	//while (true)
	//{
	//	i = 0;
	//	res = 0;
	//	while (res == 0)
	//	{
	//		res = GetMSG(devHandle, &data_r);
	//		tvk = static_cast<TUVK>(g_GetMaskedValue(data_r, 0xF0));
	//		asp = static_cast<ASP>(g_GetMaskedValue(data_r, 0x0F));
	//	}

	//	//if (tvk != TUVK::UNKN && asp != ASP::UN)
	//	printf("TUVK %d ASP %d\n", tvk, asp);
	//	data_s = data_r;
	//	res = SendMSG(devHandle, data_s);
	//	printf("SEND %d\n", data_s);
	//	data_r = 0;
	//	while (data_r != SYNC)
	//	{
	//		i++;
	//		usleep(50 * 1000);
	//		res = GetMSG(devHandle, &data_r);
	//		if (i > 100)
	//		{
	//			printf("GET %x\n", data_r);
	//			printf("TO\n");
	//			return;
	//		}
	//	}
	//}
	//return;
	while (true)
	{
		printf("LOOP\n");
		if (!was_heandshaked)
		{
			i = 0;
			res = SendMSG(devHandle, heandshake_s);
			while (heandshake_r != HELLO_GET)
			{
				usleep(50 * 1000);
				heandshake_r = 0;
				res = GetMSG(devHandle, &heandshake_r);
				i++;
				//if (heandshake_r != 0x0)
				printf("HENDSHAKE %x\n", heandshake_r);
				if (i > 10) // device unplaged or TO
				{
					printf("TimeOut\n");
					was_heandshaked = false;
					return;
				}

			}
			was_heandshaked = true;
		}
		//END HENDSHAKE
		//usleep(1000 * 300);

		i = 0;
		while (data_r != SYNC)
		{
			data_r = 0;
			res = GetMSG(devHandle, &data_r); //ждем синк сигнал
			printf("%d SYNC1 RECIVED %x\n",i, data_r);
			usleep(100 * 1000);
			i++;
			if (i > 20) // device unplaged or TO
			{
				printf("TimeOut SYNC 1\n");
				was_heandshaked = false;
				return;
			}
		}
		printf("send SYNC 1\n");
		res = SendMSG(devHandle, SYNC);//шлем СИНК
		//usleep(10 * 1000);
		res = 0;
		data_r = 0;
		printf("read data\n");
		while (data_r == 0 || data_r == SYNC || data_r == HELLO_GET)
		{
			res = GetMSG(devHandle, &data_r);
			//
			//printf("DATA RECIVED %x\n", data_r);
		}
		 //ждем данные
		printf("RECIVED %x\n", data_r);
		tvk = static_cast<TUVK>(g_GetMaskedValue(data_r, 0xF0));
		asp = static_cast<ASP>(g_GetMaskedValue(data_r, 0x0F));
		if (tvk != TUVK::UNKN && asp != ASP::UN)
			printf("TUVK %d ASP %d\n", tvk, asp);
		data_s = data_r;
		i = 0;
		printf("wait SYNC 2\n");
		while (data_r != SYNC)
		{
			res = GetMSG(devHandle, &data_r); //ждем синк сигнал
			usleep(100 * 1000);
			i++;
			if (i > 20)
			{
				printf("TimeOut SYNC 2\n");
				was_heandshaked = false;
				return;
			}
		}
		printf("send SYNC 2\n");
		res = SendMSG(devHandle, SYNC);//шлем СИНК
		usleep(100 * 1000);
		printf("send DATA %x\n", data_s);
		res = SendMSG(devHandle, data_s); //отправляем квиток

		//printf("SENDED %x\n", data_s);
		//DEBUG
		res = 1;
		printf("CLEAR BUFFER\n");
		while (res > 0)
		{
			//освобождаем буфер, хуяча со скоростью процессора
			data_r = 0;
			res = GetMSG(devHandle, &data_r);
		}
		//
		data_r = data_s = heandshake_r = 0;
	}
	return;
}
int main(int argc, char* argv[])
{
	was_open = false;
	was_heandshaked = false;
	usb_dev_handle* devHandle = NULL;
	usb_init();
	while (true)
	{
		while (TryWakeUp(devHandle) != 1)
		{
			printf("DIGI NF.. SLEEP\n");
			usleep(1000 * 1000);
		}

		Worker(devHandle);
	}
	//res = usb_release_interface(devHandle, interface->bInterfaceNumber);
	//usb_close(devHandle);
	return 1;
}