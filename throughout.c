#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "usb/usb_hid.h"
#include "bt/bt_hid.h"
#include "bt/utils/bt_utils.h"

//#define DEST_ADDR "b8:8a:60:6a:68:d6"
#define DEST_ADDR "60:BE:B5:30:61:AB"
#define PREFIX "\xa1\x12"

#define USB_INPUT_SIZE 8
#define BT_INPUT_SIZE 64
#define USB_BUFFER_SIZE (USB_INPUT_SIZE + sizeof(PREFIX) - 1)

static pthread_mutex_t mutex;
	
static void *usb_loop(void *data)
{
    int interrupt_socket = *(int*)data;
    usb_hid_ctx *ctx = usb_hid_init(0x046d, 0xc52f, 0);

	if (ctx == NULL)
	{
		return NULL;
	}

    printf("Connected to USB device\n");

	for (;;)
	{
		char input[USB_INPUT_SIZE];
		
		if (usb_hid_get_report(ctx, 0x81,
					input, USB_INPUT_SIZE))
		{
			char buffer[USB_BUFFER_SIZE];

			strcpy(buffer, PREFIX);
			memcpy(buffer + sizeof(PREFIX) - 1, input, USB_INPUT_SIZE);

			write(interrupt_socket, buffer,
				USB_BUFFER_SIZE);
		}
	}

    return NULL;
}

static void *bt_loop(void *data)
{
    int interrupt_socket = *(int*)data;
    bt_hid_ctx *ctx = bt_hid_init();

	if (ctx == NULL)
	{
		return NULL;
	}
    
    printf("Connected to BT device\n");

	for (;;)
	{
		char input[BT_INPUT_SIZE];
        int bytesRead;
		
		if ((bytesRead = bt_hid_get_report(ctx, input, BT_INPUT_SIZE)))
		{
            pthread_mutex_lock(&mutex);
			write(interrupt_socket, input, bytesRead);
            pthread_mutex_unlock(&mutex);
		}
	}

    return NULL;
}

int main(int argc, char *argv[])
{
	int control_socket;
	int interrupt_socket;

    pthread_t bt_thread, usb_thread;

    control_socket = bt_connect_to_psm(DEST_ADDR, 17);

    if (control_socket >= 0)
    {
	    printf("Success on control socket\n");
	}
    else
    {
	    return -1;
	}
	
    interrupt_socket = bt_connect_to_psm(DEST_ADDR, 19);

    if (interrupt_socket >= 0)
    {
	    printf("Success on interrupt socket\n");
	}
    else
    {
	    return -1;
	}

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&bt_thread, NULL, bt_loop, &interrupt_socket);
    pthread_create(&usb_thread, NULL, usb_loop, &interrupt_socket);

    pthread_join(bt_thread, NULL);
    pthread_join(usb_thread, NULL);

	close(interrupt_socket);

	close(control_socket);

	return 0;
}
