#include "CmakeUSB.h"
#include <iostream>
//bool was_open;
//libusb_context* ctx;
//int TryWakeUp(libusb_device_handle*& outdevHandle)
//{
//	bool was_found = false;
//	libusb_device** list = NULL;
//	int count = libusb_get_device_list(ctx, &list);
//	int rc = 0;
//	for (size_t idx = 0; idx < count; ++idx)
//	{
//		libusb_device* device = list[idx];
//		libusb_device_descriptor desc = { 0 };
//		rc = libusb_get_device_descriptor(device, &desc);
//		if ((desc.idVendor == 0x16c0) && (desc.idProduct == 0x05df))
//		{
//			was_found = true;
//			printf("Detected DigiSpark... \n");
//			if (!was_open)
//				if (libusb_open(device, &outdevHandle) == 0)
//				{
//					int config2 = 0;
//					rc = libusb_get_configuration(outdevHandle, &config2);
//					printf("\nConfigured value: %d", config2);
//					if (config2 != 1)
//					{
//						libusb_set_configuration(outdevHandle, 0);
//						if (rc != 0)
//						{
//							printf("Error in libusb_set_configuration\n");
//							libusb_free_device_list(list, 1);
//							libusb_close(outdevHandle);
//							return 0;
//						}
//						else
//							printf("\nDevice is in configured state!");
//					}
//					if (libusb_kernel_driver_active(outdevHandle, 0) == 1)
//					{
//						printf("\nKernel Driver Active");
//						if (libusb_detach_kernel_driver(outdevHandle, 0) == 0)
//							printf("\nKernel Driver Detached!");
//						else
//						{
//							printf("\nCouldn't detach kernel driver!\n");
//							libusb_free_device_list(list, 1);
//							libusb_close(outdevHandle);
//							return -1;
//						}
//					}
//					rc = libusb_claim_interface(outdevHandle, 1);
//					if (rc < 0)
//					{
//						printf("\nCannot Claim Interface");
//						libusb_free_device_list(list, 1);
//						libusb_close(outdevHandle);
//						return -1;
//					}
//					else
//						printf("\nClaimed Interface\n");
//
//					active_config(device, outdevHandle);
//
//					was_open = true;
//					break;
//				}
//		}
//	}
//	if (!was_found && was_open)
//	{
//		was_open = false;
//		libusb_close(outdevHandle);
//	}
//	libusb_free_device_list(list, 1);
//	if (was_found && was_open)
//		return 1;
//	return 0;
//}
//
//void Worker(libusb_device_handle* devHandle)
//{
//	int res;
//	unsigned char heandshake_r = 0x0;
//	unsigned char heandshake_s = HELLO_SEND;
//	unsigned char data_r = 0x0;
//	unsigned char data_s = 0x0;
//	res = -1;
//	int i = 0;
//	TUVK tvk = TUVK::UNKN;
//	ASP asp = ASP::UN;
//	unsigned char* transfer_out, * transfer_in;
//	transfer_out = (unsigned char*)malloc(63 + 1);
//	transfer_in = (unsigned char*)malloc(63 + 1);
//	int transferred = 0;
//	int received = 0;
//	int length =63;
//	memset(transfer_out, '\0', 64);
//	memset(transfer_in, '\0', 64);
//	transfer_out[1] = HELLO_SEND;
//	while (true)
//	{
//		printf("LOOP\n");
//		i = 0;
//		while (heandshake_r != HELLO_GET)
//		{
//			res = libusb_control_transfer(devHandle, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS, 0x01, 0, 0, transfer_in, 4, 0);
//			res = GetMSG(devHandle, &heandshake_r);
//			res = SendMSG(devHandle, heandshake_s);
//			res = libusb_control_transfer(devHandle, MSG_GET,0x01,0,0,transfer_in,64,0);
//
//			//res = libusb_bulk_transfer(devHandle, 0x0fd, transfer_out, length, &transferred, 0);
//			if (res == 0 && transferred == length)
//			{
//				printf("\nWrite successful!");
//				printf("\nSent %d bytes with string: %s\n", transferred, transfer_out);
//			}
//			else
//				printf("\nError in write! e = %d and transferred = %d\n", res, transferred);
//
//#ifdef WIN32
//			std::this_thread::sleep_for(std::chrono::milliseconds(100));
//#else
//			usleep(1000 * 100);
//#endif // WIN32
//			i = 0;
//
//			for (i = 0; i < length; i++)
//			{
//				res = libusb_bulk_transfer(devHandle, 0x82, transfer_in, 64, &received, 0);  //64: Max Packet Length
//				if (res == 0)
//				{
//					printf("\nReceived: ");
//					printf("%c", transfer_in[i]); //Will read a string from LPC2148
//					//sleep(1);
//				}
//				else
//				{
//					printf("\nError in read! e = %d and received = %d\n", res, received);
//				}
//			}
//
//
//
//
//
//
//
//
//			res = SendMSG(devHandle, heandshake_s); //параметр задан неверно!
//			//https://stackoverflow.com/questions/52949762/libusb-c-format-of-usb-transfer-differs
//#ifdef WIN32
//			std::this_thread::sleep_for(std::chrono::milliseconds(100));
//#else
//			usleep(1000 * 100);
//#endif // WIN32
//			unsigned char* test;
//			test = (unsigned char*)malloc(sizeof(unsigned char) * 4);
//			memset(test, 0, sizeof(unsigned char) * 4);
//			res = GetMSG(devHandle, test);
//			//res = GetMSG(devHandle, &heandshake_r);
//			i++;
//			printf("HENDSHAKE %x\n", heandshake_r);
//			if (i > 10) // device unplaged
//			{
//				//usb_close(devHandle);
//				return;
//			}
//			}
//		//END HENDSHAKE
//		//usleep(1000 * 300);
//		res = GetMSG(devHandle, &data_r);
//		printf("RECIVED %x\n", data_r);
//
//		tvk = static_cast<TUVK>(g_GetMaskedValue(data_r, 0xF0));
//		asp = static_cast<ASP>(g_GetMaskedValue(data_r, 0x0F));
//		printf("TUVK %d ASP %d\n", tvk, asp);
//		data_s = data_r;
//		//usleep(1000 * 100);
//		res = SendMSG(devHandle, data_s);
//		printf("SENDED %x\n", data_s);
//		data_r = data_s = heandshake_r = 0;
//		}
//	return;
//	}
//
//int main(int argc, char* argv[])
//{
//	setlocale(LC_ALL, "");
//	was_open = false;
//	int res = 0;
//	libusb_context* ctx = NULL;
//	res = libusb_init(&ctx);
//	libusb_set_debug(ctx, 4);
//	libusb_device_handle* devHandle = NULL;
//	while (true)
//	{
//		while (TryWakeUp(devHandle) != 1)
//		{
//
//#ifdef WIN32
//
//			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//			printf("DIGI NF.. SLEEP\n");
//#else
//			printf("DIGI NF.. SLEEP\n");
//			usleep(1000 * 1000);
//#endif
//	}
//		Worker(devHandle);
//		//printf("ONLINE\n");
//}
//
//	libusb_exit(NULL);
//	return 1;
//}
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
	int res;
	uint8_t heandshake_r = 0x0;
	uint8_t heandshake_s = HELLO_SEND;
	uint8_t data_r = 0x0;
	uint8_t data_s = 0x0;
	res = -1;
	int i = 0;
	TUVK tvk = TUVK::UNKN;
	ASP asp = ASP::UN;
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
			res = GetMSG(devHandle, &data_r); //ждем синк сигнал
			//printf("%d SYNC RECIVED %x\n",i, data_r);
			usleep(100 * 1000);
			i++;
			if (i > 20) // device unplaged or TO
			{
				printf("TimeOut SYNC 1\n");
				was_heandshaked = false;
				return;
			}
		}
		res = SendMSG(devHandle, SYNC);//шлем СИНК
		//usleep(10 * 1000);
		res = 0;
		while (res == 0)
		{
			res = GetMSG(devHandle, &data_r); //ждем данные
		}

		//printf("RECIVED %x\n", data_r);И
		tvk = static_cast<TUVK>(g_GetMaskedValue(data_r, 0xF0));
		asp = static_cast<ASP>(g_GetMaskedValue(data_r, 0x0F));
		if (tvk != TUVK::UNKN && asp != ASP::UN)
			printf("TUVK %d ASP %d\n", tvk, asp);
		data_s = data_r;

		res = SendMSG(devHandle, SYNC);//шлем СИНК
		i = 0;
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
		res = SendMSG(devHandle, data_s); //отправляем квиток
		//printf("SENDED %x\n", data_s);
		//DEBUG
		res = 1;
		while (res > 0)
		{
			//usleep(1000 * 100);
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