#include <stdlib.h>
#include <libusb.h>
#include "usb_hid.h"

struct _usb_hid_ctx
{
	libusb_context *ctx;
	libusb_device_handle *handle;
};

usb_hid_ctx *usb_hid_init(uint16_t vid, uint16_t pid, uint16_t interface)
{
	usb_hid_ctx *result = NULL;

	libusb_context *ctx;
	libusb_device_handle *handle;

	libusb_init(&ctx);

	handle = libusb_open_device_with_vid_pid(ctx,
			vid, pid);
	
	if (handle)
	{
		libusb_set_auto_detach_kernel_driver(handle, 1);
		if (LIBUSB_SUCCESS ==
				libusb_claim_interface(handle, interface))
		{
			result = (usb_hid_ctx*)malloc(sizeof(usb_hid_ctx));
			result->ctx = ctx;
			result->handle = handle;
		}
	}

	return result;
}

void usb_hid_exit(usb_hid_ctx *ctx)
{
	libusb_exit(ctx->ctx);
}

int usb_hid_get_report(usb_hid_ctx *ctx,
		uint16_t endpoint, uint8_t *report, int size)
{
	int actual_length = 0;

	int result;

	result = libusb_interrupt_transfer(
		ctx->handle, endpoint, report, size,
		&actual_length, 5000);

	if (result < 0)
	{
		actual_length = 0;
	}

	return actual_length;
}
