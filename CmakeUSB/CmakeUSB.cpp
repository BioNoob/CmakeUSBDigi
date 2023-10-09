#include "CmakeUSB.h"
bool was_open;

int TryWakeUp(libusb_device_handle*& outdevHandle)
{
	bool was_found = false;
	libusb_device** list = NULL;
	int count = libusb_get_device_list(NULL, &list);
	int rc = 0;
	for (size_t idx = 0; idx < count; ++idx)
	{
		libusb_device* device = list[idx];
		libusb_device_descriptor desc = { 0 };
		rc = libusb_get_device_descriptor(device, &desc);
		if ((desc.idVendor == 0x16c0) && (desc.idProduct == 0x05df))
		{
			was_found = true;
			printf("Detected DigiSpark... \n");
			if (!was_open)
				if (libusb_open(device, &outdevHandle) == 0)
				{
					libusb_claim_interface(outdevHandle, 0);
					was_open = true;
					break;
				}
		}
	}
	if (!was_found && was_open)
	{
		was_open = false;
		libusb_close(outdevHandle);
	}
	libusb_free_device_list(list, 1);
	if (was_found && was_open)
		return 1;
	return 0;
}

void Worker(libusb_device_handle* devHandle)
{
	int res;
	unsigned char heandshake_r = 0x0;
	unsigned char heandshake_s = HELLO_SEND;
	unsigned char data_r = 0x0;
	unsigned char data_s = 0x0;
	res = -1;
	int i = 0;
	TUVK tvk = TUVK::UNKN;
	ASP asp = ASP::UN;
	while (true)
	{
		printf("LOOP\n");
		i = 0;
		while (heandshake_r != HELLO_GET)
		{
			res = SendMSG(devHandle, heandshake_s);
#ifdef WIN32
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
#else
			usleep(1000 * 100);
#endif // WIN32
			res = GetMSG(devHandle, &heandshake_r);
			i++;
			printf("HENDSHAKE %x\n", heandshake_r);
			if (i > 10) // device unplaged
			{
				//usb_close(devHandle);
				return;
			}
		}
		//END HENDSHAKE
		//usleep(1000 * 300);
		res = GetMSG(devHandle, &data_r);
		printf("RECIVED %x\n", data_r);

		tvk = static_cast<TUVK>(g_GetMaskedValue(data_r, 0xF0));
		asp = static_cast<ASP>(g_GetMaskedValue(data_r, 0x0F));
		printf("TUVK %d ASP %d\n", tvk, asp);
		data_s = data_r;
		//usleep(1000 * 100);
		res = SendMSG(devHandle, data_s);
		printf("SENDED %x\n", data_s);
		data_r = data_s = heandshake_r = 0;
	}
	return;
}

int main(int argc, char* argv[])
{
	was_open = false;
	int res = 0;
	res = libusb_init(NULL);
	libusb_device_handle* devHandle = NULL;
	while (true)
	{
		while (TryWakeUp(devHandle) != 1)
		{

#ifdef WIN32

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			printf("DIGI NF.. SLEEP\n");
#else
			printf("DIGI NF.. SLEEP\n");
			usleep(1000 * 1000);
#endif
		}
		Worker(devHandle);
		//printf("ONLINE\n");
	}

	libusb_exit(NULL);
	return 1;
}















//int TryWakeUp(usb_dev_handle*& outdevHandle)
//{
//	struct usb_bus* bus = NULL;
//	int numInterfaces = 0;
//	struct usb_device* digiSpark = NULL;
//	struct usb_device* device = NULL;
//	usb_init();
//	// Enumerate the USB device tree
//	usb_find_busses();
//	usb_find_devices();
//
//	bool _breaker = false;
//	// Iterate through attached busses and devices
//	bus = usb_get_busses();
//	while (bus != NULL)
//	{
//		if (_breaker)
//			break;
//		device = bus->devices;
//		while (device != NULL)
//		{
//			// Check to see if each USB device matches the DigiSpark Vendor and Product IDs
//			if ((device->descriptor.idVendor == 0x16c0) && (device->descriptor.idProduct == 0x05df))
//			{
//				printf("Detected DigiSpark... \n");
//				digiSpark = device;
//				_breaker = true;
//				break;
//			}
//			device = device->next;
//		}
//		bus = bus->next;
//	}
//	if (digiSpark == NULL)
//		return 0;
//	struct usb_dev_handle* devHandle = NULL;
//	struct usb_interface_descriptor* interface = NULL;
//	devHandle = usb_open(digiSpark);
//	if (devHandle != NULL)
//	{
//		numInterfaces = digiSpark->config->bNumInterfaces;
//		interface = &(digiSpark->config->interface[0].altsetting[0]);
//		printf("Found %i interfaces, using interface %i\n", numInterfaces, interface->bInterfaceNumber);
//		outdevHandle = devHandle;
//	}
//	else
//		return 0;
//	//END SETTINGS
//	return 1;
//}
//void Worker(usb_dev_handle* devHandle)
//{
//	int res;
//	char heandshake_r = 0x0;
//	char heandshake_s = HELLO_SEND;
//	char data_r = 0x0;
//	char data_s = 0x0;
//	res = -1;
//	int i = 0;
//	TUVK tvk = UNKN;
//	ASP asp = UN;
//	while (true)
//	{
//		printf("LOOP\n");
//		i = 0;
//		while (heandshake_r != HELLO_GET)
//		{
//			res = SendMSG(devHandle, heandshake_s);
//			//usleep(1000 * 100);
//			res = GetMSG(devHandle, &heandshake_r);
//			i++;
//		//	printf("HENDSHAKE %x\n", heandshake_r);
//			if (i > 10) // device unplaged
//			{
//				usb_close(devHandle);
//				return;
//			}
//				
//		}
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
//	}
//	return;
//}
//int main(int argc, char* argv[])
//{
//
//	usb_dev_handle* devHandle = NULL;
//	while (true)
//	{
//		while (TryWakeUp(devHandle) != 1)
//		{
//			printf("DIGI NF.. SLEEP\n");
//			//usleep(1000 * 1000);
//		}
//		Worker(devHandle);
//	}
//	//res = usb_release_interface(devHandle, interface->bInterfaceNumber);
//	//usb_close(devHandle);
//	return 1;
//}