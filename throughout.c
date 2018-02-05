#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "usb/usb_hid.h"
#include "bt/bt_hid.h"
#include "bt/utils/bt_utils.h"

#define CLIENT0_ADDRESS "60:BE:B5:30:61:AB"
#define CLIENT1_ADDRESS "b8:8a:60:6a:68:d6"
#define PREFIX "\xa1\x12"

#define USB_INPUT_SIZE 8
#define BT_INPUT_SIZE 64
#define USB_BUFFER_SIZE (USB_INPUT_SIZE + sizeof(PREFIX) - 1)

#define NUM_OF_CLIENTS 4

#define SWITCH_SEQUENCE "\xa1\x01\x22\x00\x00\x00\x00\x00\x00\x00"

static struct client
{
    const char *address;
    int control_socket;
    int interrupt_socket;
} clients[NUM_OF_CLIENTS];

static struct client *current_client = NULL;

static pthread_mutex_t mutex;

static void switch_current_client()
{
    int i, j;
    // Find current client
    for (i = 0; i < NUM_OF_CLIENTS; i++)
    {
        if (&clients[i] == current_client)
        {
            close(clients[i].control_socket);
            close(clients[i].interrupt_socket);
            break;
        }
    }

    // Find next connected client
    for (j = (i + 1) % NUM_OF_CLIENTS; j != i; j = (j + 1) % NUM_OF_CLIENTS)
    {
        if (clients[j].address)
        {
                clients[j].control_socket =
                    bt_connect_to_psm(clients[j].address, 17);

                if (clients[j].control_socket >= 0)
                {
                    printf("Success on control socket for client %d\n", j + 1);
                }
                else
                {
                    continue;
                }
        
                clients[j].interrupt_socket =
                    bt_connect_to_psm(clients[j].address, 19);

                if (clients[j].interrupt_socket >= 0)
                {
                    printf("Success on interrupt socket for client %d\n",
                        j + 1);

                    current_client = &clients[j];
                    break;
                }
                else
                {
                    continue;
                }
        }
    }
}

static void send_to_client(const char *input, int size)
{
    if (current_client != NULL)
    {
        pthread_mutex_lock(&mutex);
        write(current_client->interrupt_socket, input, size);
        pthread_mutex_unlock(&mutex);
    }
}
	
static void *usb_loop(void *data)
{
    char buffer[USB_BUFFER_SIZE];
    usb_hid_ctx *ctx = usb_hid_init(0x046d, 0xc52f, 0);

	if (ctx == NULL)
	{
		return NULL;
	}

    printf("Connected to USB device\n");

    strcpy(buffer, PREFIX);

	for (;;)
	{
		char input[USB_INPUT_SIZE];
		
		if (usb_hid_get_report(ctx, 0x81,
					input, USB_INPUT_SIZE))
		{
			memcpy(buffer + sizeof(PREFIX) - 1, input, USB_INPUT_SIZE);

			send_to_client(buffer, USB_BUFFER_SIZE);
		}
	}

    return NULL;
}

static void *bt_loop(void *data)
{
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
            if (strncmp(input, SWITCH_SEQUENCE, bytesRead) == 0)
            {
                switch_current_client();
            }

			send_to_client(input, bytesRead);
		}
	}

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t bt_thread, usb_thread;

    memset(clients, 0, sizeof(clients));

    clients[0].address = CLIENT0_ADDRESS;
    clients[1].address = CLIENT1_ADDRESS;

    switch_current_client();

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&bt_thread, NULL, bt_loop, NULL);
    pthread_create(&usb_thread, NULL, usb_loop, NULL);

    pthread_join(bt_thread, NULL);
    pthread_join(usb_thread, NULL);

    if (current_client != NULL)
    {
        close(current_client->control_socket);
        close(current_client->interrupt_socket);
    }	

	return 0;
}
