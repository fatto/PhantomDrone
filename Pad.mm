#include "Pad.hpp"

//#include <Cocoa/Cocoa.h>
#include <iostream>

Pad::Pad()
{
	hidManager = IOHIDManagerCreate( kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	
	
	IOHIDManagerSetDeviceMatching(hidManager, NULL);
	IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
	CFSetRef device_set = IOHIDManagerCopyDevices(hidManager);
	CFIndex number_of_devices = device_set == NULL ? 0 : CFSetGetCount(device_set);
	const IOHIDDeviceRef *device_array;
	const IOHIDDeviceRef *current;
	CFNumberRef vendor, product;
	long vendor_id, product_id;
	CFStringRef manufacturer, product_name;
	char string_buffer[1024];
	/*
	 Tell the user how many we found
	 */
	if (number_of_devices == 0)
		printf("No HID devices detected\n");
	else
	{
		printf("%lld HID devices found\n", (long long)number_of_devices);
		/*
		 Get the list into a C++ array
		 */
		device_array = new IOHIDDeviceRef [number_of_devices];
		CFSetGetValues(device_set, (const void **)device_array);
		
		/*
		 Iterate the device list
		 */
		for (current = device_array; current < device_array + number_of_devices; current++)
		{
			vendor_id = product_id = 0;
			
			/*
			 Get the vendor ID (which is a 32-bit integer)
			 */
			if ((vendor = (CFNumberRef)IOHIDDeviceGetProperty(*current, CFSTR(kIOHIDVendorIDKey))) != NULL)
				CFNumberGetValue(vendor, kCFNumberSInt32Type, &vendor_id);
			printf("VID:%04lX ", vendor_id);
			
			/*
			 Get the product ID (which is a 32-bit integer)
			 */
			if ((product = (CFNumberRef)IOHIDDeviceGetProperty(*current, CFSTR(kIOHIDProductIDKey))) != NULL)
				CFNumberGetValue((CFNumberRef)product, kCFNumberSInt32Type, &product_id);
			printf("PID:%04lX ", product_id);
			
			/*
			 Get the manufacturer name (which is a string)
			 */
			if ((manufacturer = (CFStringRef)IOHIDDeviceGetProperty(*current, CFSTR(kIOHIDManufacturerKey)))!= NULL)
			{
				CFStringGetCString(manufacturer, string_buffer, sizeof(string_buffer), kCFStringEncodingUTF8);
				printf("%s ", string_buffer);
			}
			
			/*
			 Get the product name (which is a string)
			 */
			if ((product_name = (CFStringRef)IOHIDDeviceGetProperty(*current, CFSTR(kIOHIDProductKey))) != NULL)
			{
				CFStringGetCString(product_name, string_buffer, sizeof(string_buffer), kCFStringEncodingUTF8);
				printf("(%s)", string_buffer);
			}
			
			puts("");
		}
		
		/*
		 We're finished with the device set and device list so free them.
		 */
		CFRelease(device_set);
		delete [] device_array;
	}
	
//	const void *keys[] = {kIOHIDDeviceUsagePageKey, kIOHIDDeviceUsageKey};
//	const void *values[] = {(CFStringRef)kHIDPage_GenericDesktop, (CFStringRef)kHIDUsage_GD_GamePad};
//	const void *keys[] = {CFStringRef(kIOHIDDeviceUsagePageKey), CFStringRef(kIOHIDDeviceUsageKey)};
//	const void *values[] = {CFNumberRef(kHIDPage_GenericDesktop), CFNumberRef( kHIDUsage_GD_GamePad)};
	CFMutableDictionaryRef criterion = CFDictionaryCreateMutable(kCFAllocatorDefault, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	
	UInt32 usagePageInt = kHIDPage_GenericDesktop;
	CFNumberRef usagePage = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePageInt);
	CFDictionarySetValue(criterion, CFSTR(kIOHIDDeviceUsagePageKey), usagePage);
	
	UInt32 pageInt = kHIDUsage_GD_GamePad;
	CFRelease(usagePage);
	CFNumberRef usage = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &pageInt);
	CFDictionarySetValue(criterion, CFSTR(kIOHIDDeviceUsageKey), usage);
	CFRelease(usage);
	
	IOHIDManagerSetDeviceMatching(hidManager, criterion);
	
	IOHIDManagerRegisterDeviceMatchingCallback(hidManager, &Pad::gamepadWasAdded, (void*)this);
	IOHIDManagerRegisterDeviceRemovalCallback(hidManager, &Pad::gamepadWasRemoved, (void*)this);
	
	IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	
	IOReturn tIOReturn = IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
	assert(tIOReturn == kIOReturnSuccess);
//	IOHIDManagerRegisterInputValueCallback(hidManager, gamepadAction, (void*)self);	
}
Pad::~Pad()
{
	IOReturn tIOReturn = IOHIDManagerClose(hidManager, kIOHIDOptionsTypeNone);
	assert(tIOReturn == kIOReturnSuccess);
}


Pad::StatusStruct Pad::Status()
{
	return Pad::StatusStruct{state.button, state.position, state.gimbal};
}
void Pad::Force(std::array<double, 3> f)
{
	
}

void Pad::gamepadWasAdded(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device)
{
	Pad& p = *static_cast<Pad*>(inContext);
	p.state.button[0] = true;
	p.state.position = {1.0f, 2.0f, 3.0f};
	std::cout << "added :D" << std::endl;
}
void Pad::gamepadWasRemoved(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device)
{
	Pad& p = *static_cast<Pad*>(inContext);
	p.state.button[0] = false;
	std::cout << "removed :[" << std::endl;
}
void Pad::updateStatus()
{
	
}
