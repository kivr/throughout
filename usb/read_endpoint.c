#include <libusb.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	libusb_context *ctx;
	libusb_device_handle *handle;

	libusb_init(&ctx);

	handle = libusb_open_device_with_vid_pid(ctx,
			0x046d, 0xc52f);

	if (!handle)
	{
		printf("Failure\n");
		return -1;
	}

	libusb_set_auto_detach_kernel_driver(handle, 1);
	if (LIBUSB_SUCCESS == libusb_claim_interface(handle, 0))
	{
		int i;
		int actual_length;

		uint8_t hid_descriptor[512];

		actual_length = libusb_control_transfer(handle,
				LIBUSB_ENDPOINT_IN
					| LIBUSB_REQUEST_TYPE_STANDARD
					| LIBUSB_RECIPIENT_INTERFACE,
				LIBUSB_REQUEST_GET_DESCRIPTOR,
				(LIBUSB_DT_REPORT << 8) | 0x00,
				0,
				hid_descriptor,
				512,
				5000);

		if (actual_length > 0)
		{
			for (i = 0; i < actual_length; i++)
			{
				fprintf(stderr, "%02x,", hid_descriptor[i]);
			}

			fprintf(stderr, "\n");
		}

		for (;;)
		{
			uint8_t report[8];
			int result;

			result = libusb_interrupt_transfer(
				handle, 0x81, report, 8,
				&actual_length, 5000);

			if (result >= 0)
			{
				for (i = 0; i < actual_length; i++)
				{
					printf("%02x", report[i]);
				}

				printf("\n");
			}
		}
	}

	libusb_exit(ctx);

	return 0;
}
