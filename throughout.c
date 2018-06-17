#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "bt/bt_hid.h"
#include "bt/utils/bt_utils.h"

#define MOUSE_REPORT_ID 20

#define SWITCH_SEQUENCE "\xa1\x01\x22\x00\x00\x00\x00\x00\x00\x00"
#define KEY_UP_SEQUENCE "\xa1\x01\x00\x00\x00\x00\x00\x00\x00\x00"

#ifdef APPLE_KEYBOARD
    #define FN_SEQUENCE "\xa1\x11\x10"
    #define FN_KEY_UP_SEQUENCE "\xa1\x11\x00"
#endif

typedef struct _tgot_client
{
    const char *address;
    int control;
    int interrupt;
    bool connected;
}tgot_client;

typedef struct _tgot_ctx
{
    tgot_client *clients;
    int selected_client;
#ifdef APPLE_KEYBOARD
    bool fnOn;
#endif
}tgot_ctx;

static bdaddr_t keyboardAddress;
static bdaddr_t mouseAddress;
//static char lastMouseInput[9];

static void check_for_control_command(int control_socket)
{
    unsigned char input[] = {0};
    unsigned char output[] = {0};
    
    if (recv(control_socket, input, sizeof(input), 0) > 0)
    {
        write(control_socket, output, sizeof(output));
    }
}

static bool create_client_connection(tgot_ctx *ctx)
{
    tgot_client *client = ctx->clients + ctx->selected_client;
    const char *address = client->address;

    client->control = bt_connect_to_psm(address, 17);

    if (client->control < 0)
    {
        return false;
    }
    
    printf("Success on control socket for client %s\n", address);

    client->interrupt = bt_connect_to_psm(address, 19);

    if (client->interrupt < 0)
    {
        close(client->control);
        return false;
    }
    
    printf("Success on interrupt socket for client %s\n", address);
    client->connected = true;
    
    check_for_control_command(client->control);

    return true;
}

static void close_client_connection(tgot_client *client)
{
    client->connected = false;
    close(client->control);
    close(client->interrupt);
}

static void switch_current_client(tgot_ctx *ctx)
{
    tgot_client *client = ctx->clients + ctx->selected_client;

    send(client->interrupt, KEY_UP_SEQUENCE, sizeof(KEY_UP_SEQUENCE) - 1, 0);

    ctx->selected_client++;
    
    // Go back to first client if we are at the end of the list
    if (ctx->clients[ctx->selected_client].address == NULL)
    {
        ctx->selected_client = 0;
    }
    
    // Close current client
    if (client->connected)
    {
        close_client_connection(client);
    }
}

static void loop_until_connected(tgot_ctx *ctx)
{
    tgot_client *client = ctx->clients + ctx->selected_client;
    while (!client->connected)
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
        tgot_client *client;

        // Wait for connection 
        loop_until_connected(ctx);
        
        client = ctx->clients + ctx->selected_client;

        result = send(client->interrupt, input, size, 0);

        if (result == -1)
        {
            close_client_connection(client);
        }
    }
}

#ifdef APPLE_KEYBOARD
static int applyFnTransform(char *input, int bytesRead)
{
    int result = bytesRead;

    if (bytesRead > 4)
    { 
        switch (input[4])
        {
            case 0x2a: // Backspace
            {
                input[4] = 0x4c; // Delete
            }
            break;
            
            case 0x50: // Left arrow
            {
                input[4] = 0x4a; // Home
            }
            break;
            
            case 0x52: // Up arrow
            {
                input[4] = 0x4b; // PgUp
            }
            break;
            
            case 0x4f: // Right arrow
            {
                input[4] = 0x4d; // End
            }
            break;
            
            case 0x51: // Down arrow
            {
                input[4] = 0x4e; // PgDown
            }
            break;

            default:
                break;
        }
    }

    return result;
}
#endif

static void bt_callback(char *input, int bytesRead, bdaddr_t *client,
                        channel_t channel, void *data)
{
    tgot_ctx *p_tgot_ctx = (tgot_ctx*)data;

    if (channel == INTERRUPT_CHANNEL)
    {
        if (memcmp(client, &keyboardAddress, sizeof(bdaddr_t)) == 0)
        {
            if (strncmp(input, SWITCH_SEQUENCE, bytesRead) == 0)
            {
                switch_current_client(p_tgot_ctx);
            }

    #ifdef APPLE_KEYBOARD
            if (strncmp(input, FN_SEQUENCE, bytesRead) == 0)
            {
                p_tgot_ctx->fnOn = true;
            }
            else if (strncmp(input, FN_KEY_UP_SEQUENCE, bytesRead) == 0)
            {
                p_tgot_ctx->fnOn = false;
            }

            if (p_tgot_ctx->fnOn)
            {
                bytesRead = applyFnTransform(input, bytesRead);
            }
    #endif

            send_to_client(p_tgot_ctx, input, bytesRead);
        }
        else if (memcmp(client, &mouseAddress, sizeof(bdaddr_t)) == 0)
        {
            input[1] = MOUSE_REPORT_ID;

            //if (memcmp(input, lastMouseInput, sizeof(lastMouseInput)) != 0)
            //{
                //memcpy(lastMouseInput, input, sizeof(lastMouseInput));
                send_to_client(p_tgot_ctx, input, bytesRead);
            //}
        }
    }
}

void bt_control_callback(int fd, bdaddr_t *address, void *data)
{
    if (memcmp(address, &mouseAddress, sizeof(bdaddr_t)) == 0)
    {
        send(fd, "\x53\xF1\x02\x01", 4, 0);
    }
}

int main(int argc, char *argv[])
{
    tgot_client clients[] =
    {
        {"6c:40:08:a5:02:5d", -1, -1, false},
        {"60:BE:B5:30:61:AB", -1, -1, false},
        {NULL, 0, 0, false}
    };

    tgot_ctx ctx =
    {
        clients, 0
#ifdef APPLE_KEYBOARD
        , false
#endif
    };

    bt_hid_ctx *bt_ctx = bt_hid_init();
    
    str2ba("28:37:37:35:9f:8c", &keyboardAddress);
    str2ba("1C:36:BB:0D:E0:41", &mouseAddress);

    if (bt_ctx != NULL)
    {
        bt_hid_register_callback(bt_ctx, bt_callback, &ctx);
        bt_hid_register_control_callback(bt_ctx, bt_control_callback, &ctx);
        bt_hid_main_loop(bt_ctx);
    }
    else
    {
        printf("Can't bind with bluetooth PSM\n");
    }

    return 0;
}

