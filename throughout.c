#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "usb_hid.h"

#define DEST_ADDR "b8:8a:60:6a:68:d6"
#define PREFIX "\xa1\x12"

#define INPUT_SIZE 8
#define BUFFER_SIZE (INPUT_SIZE + sizeof(PREFIX) - 1)

int main(int argc, char *argv[])
{
	struct sockaddr_l2 control_addr;
	int control_socket, status;

	memset(&control_addr, 0, sizeof(control_addr));

	control_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);

	control_addr.l2_family = AF_BLUETOOTH;
	control_addr.l2_psm = 17;
	str2ba(DEST_ADDR, &control_addr.l2_bdaddr);

	status = connect(control_socket, (struct sockaddr*)&control_addr,
			sizeof(control_addr));

	if (status != 0)
	{
		close(control_socket);
		return -1;
	}

	struct sockaddr_l2 interrupt_addr;
	int interrupt_socket;

	printf("Success on control socket\n");
	
	memset(&interrupt_addr, 0, sizeof(interrupt_addr));

	interrupt_socket = socket(AF_BLUETOOTH, SOCK_STREAM,
		BTPROTO_L2CAP);

	interrupt_addr.l2_family = AF_BLUETOOTH;
	interrupt_addr.l2_psm = 19;
	str2ba(DEST_ADDR, &interrupt_addr.l2_bdaddr);

	status = connect(interrupt_socket,
		(struct sockaddr*)&interrupt_addr,
		sizeof(interrupt_addr));

	if (status != 0)
	{
		close(interrupt_socket);
		return -1;
	}

	printf("Success on interrupt socket\n");

	usb_hid_ctx *ctx = usb_hid_init(0x046d, 0xc52f, 0);

	if (ctx == NULL)
	{
		return -1;
	}

	for (;;)
	{
		char input[INPUT_SIZE];
		
		if (usb_hid_get_report(ctx, 0x81,
					input, INPUT_SIZE))
		{
			char buffer[BUFFER_SIZE];

			strcpy(buffer, PREFIX);
			memcpy(buffer + sizeof(PREFIX) - 1, input, INPUT_SIZE);

			write(interrupt_socket, buffer,
				BUFFER_SIZE);
		}
	}

	close(interrupt_socket);

	close(control_socket);

	return 0;
}
