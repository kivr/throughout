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

#define USB_PREFIX "\xa1\x12"
#define USB_BUFFER_SIZE (USB_INPUT_SIZE + sizeof(USB_PREFIX) - 1)

#define USB_INPUT_SIZE 8
#define BT_INPUT_SIZE 10

#define SWITCH_SEQUENCE "\xa1\x01\x22\x00\x00\x00\x00\x00\x00\x00"
#define KEY_UP_SEQUENCE "\xa1\x01\x00\x00\x00\x00\x00\x00\x00\x00"

struct _tgot_ctx
{
    const char **clients;
    int selected_client;
    int control_socket;
    int interrupt_socket;
    bool connected;
    pthread_mutex_t *mutex;
};

typedef struct _tgot_ctx tgot_ctx;

static pthread_mutex_t mutex;

static bool create_client_connection(tgot_ctx *ctx)
{
    const char *address = ctx->clients[ctx->selected_client];

    ctx->control_socket = bt_connect_to_psm(address, 17);

    if (ctx->control_socket < 0)
    {
        return false;
    }
    
    printf("Success on control socket for client %s\n", address);

    ctx->interrupt_socket = bt_connect_to_psm(address, 19);

    if (ctx->interrupt_socket < 0)
    {
        close(ctx->control_socket);
        return false;
    }
    
    printf("Success on interrupt socket for client %s\n", address);
    ctx->connected = true;

    return true;
}

static void close_client_connection(tgot_ctx *ctx)
{
    ctx->connected = false;
    close(ctx->control_socket);
    close(ctx->interrupt_socket);
}

static void switch_current_client(tgot_ctx *ctx)
{
    ctx->selected_client++;
    
    // Go back to first client if we are at the end of the list
    if (ctx->clients[ctx->selected_client] == NULL)
    {
        ctx->selected_client = 0;
    }
    
    // Close current client
    if (ctx->connected)
    {
        close_client_connection(ctx);
    }
}

static void loop_until_connected(tgot_ctx *ctx)
{
    while (!ctx->connected)
    {
        bool result = create_client_connection(ctx);
        if (!result)
        {
            switch_current_client(ctx);
        }
    }
}

static void send_to_client(tgot_ctx *ctx, const char *input, int size)
{
    int result = -1;

    while (result == -1)
    {
        // Wait for connection 
        loop_until_connected(ctx);

        result = write(ctx->interrupt_socket, input, size);

        if (result == -1)
        {
            close_client_connection(ctx);
        }
    }
}

static void *usb_loop(void *data)
{
    char buffer[USB_BUFFER_SIZE];
    tgot_ctx *p_tgot_ctx = (tgot_ctx*)data;
    usb_hid_ctx *ctx = usb_hid_init(0x046d, 0xc52f, 0);

    if (ctx == NULL)
    {
        return NULL;
    }

    printf("Connected to USB device\n");

    strcpy(buffer, USB_PREFIX);

    for (;;)
    {
        char input[USB_INPUT_SIZE];

        if (usb_hid_get_report(ctx, 0x81,
                    input, USB_INPUT_SIZE))
        {
            memcpy(buffer + sizeof(USB_PREFIX) - 1, input, USB_INPUT_SIZE);

            pthread_mutex_lock(p_tgot_ctx->mutex);
            send_to_client(p_tgot_ctx, buffer, USB_BUFFER_SIZE);
            pthread_mutex_unlock(p_tgot_ctx->mutex);
        }
    }

    return NULL;
}

static void *bt_loop(void *data)
{
    tgot_ctx *p_tgot_ctx = (tgot_ctx*)data;
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
            pthread_mutex_lock(p_tgot_ctx->mutex);
            if (strncmp(input, SWITCH_SEQUENCE, bytesRead) == 0)
            {
                switch_current_client(p_tgot_ctx);
            }

            send_to_client(p_tgot_ctx, input, bytesRead);
            pthread_mutex_unlock(p_tgot_ctx->mutex);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t bt_thread, usb_thread;
    pthread_mutex_t mutex;

    const char *clients[] = {"6c:40:08:a5:02:5d", "60:BE:B5:30:61:AB", NULL};
    tgot_ctx ctx = {clients, 0, -1, -1, false, &mutex};

    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&bt_thread, NULL, bt_loop, &ctx);
    pthread_create(&usb_thread, NULL, usb_loop, &ctx);

    pthread_join(bt_thread, NULL);
    pthread_join(usb_thread, NULL);

    close_client_connection(&ctx);

    return 0;
}
