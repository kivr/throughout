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

#define CLIENT0_ADDRESS "b8:8a:60:6a:68:d6"
#define CLIENT1_ADDRESS "60:BE:B5:30:61:AB"
#define PREFIX "\xa1\x12"

#define USB_INPUT_SIZE 8
#define BT_INPUT_SIZE 64
#define USB_BUFFER_SIZE (USB_INPUT_SIZE + sizeof(PREFIX) - 1)

#define NUM_OF_CLIENTS 4

static struct client
{
    bool connected;
    const char *address;
    int control_socket;
    int interrupt_socket;
} clients[NUM_OF_CLIENTS];

static struct client *current_client;

static pthread_mutex_t mutex;

static void connect_clients()
{
    int i;

    for (i = 0; i < NUM_OF_CLIENTS; i++)
    {
        if (!clients[i].connected && clients[i].address != NULL)
        {
            clients[i].control_socket =
                bt_connect_to_psm(clients[i].address, 17);

            if (clients[i].control_socket >= 0)
            {
                printf("Success on control socket for client %d\n", i + 1);
            }
            else
            {
                continue;
            }
	
            clients[i].interrupt_socket =
                bt_connect_to_psm(clients[i].address, 19);

            if (clients[i].interrupt_socket >= 0)
            {
                printf("Success on interrupt socket for client %d\n", i + 1);
                clients[i].connected = true;

                if (current_client == NULL)
                {
                    current_client = &clients[i];
                }
            }
            else
            {
                continue;
            }
        }
    }
}
    
static void close_clients()
{
    int i;

    for (i = 0; i < NUM_OF_CLIENTS; i++)
    {
        if (!clients[i].connected)
        {
            close(clients[i].control_socket);
            close(clients[i].interrupt_socket);
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

    connect_clients();

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&bt_thread, NULL, bt_loop, NULL);
    pthread_create(&usb_thread, NULL, usb_loop, NULL);

    pthread_join(bt_thread, NULL);
    pthread_join(usb_thread, NULL);

	close_clients();

	return 0;
}
