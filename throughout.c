#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "bt/bt_hid.h"
#include "bt/utils/bt_utils.h"

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

#define HID_PREFIX 0xa1

#define MOUSE_REPORT_ID 20
#define MOUSE_REPORT_SIZE 10

#define SWITCH_SEQUENCE "\xa1\x11\x08"
#define KEY_UP_SEQUENCE "\xa1\x01\x00\x00\x00\x00\x00\x00\x00\x00"

#ifdef APPLE_KEYBOARD
    #define FN_SEQUENCE "\xa1\x11\x10"
    #define FN_KEY_UP_SEQUENCE "\xa1\x11\x00"
#endif

typedef enum _mouse_state
{
    RELEASED,
    PRESSED,
    SCROLLING,
} mouse_state;

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
    int clientCount;
    bool switchSeqOn;
#ifdef APPLE_KEYBOARD
    bool fnOn;
#endif
    mouse_state mouseState;
    int8_t lastMouseX;
    int8_t lastMouseY;
}tgot_ctx;

static bdaddr_t keyboardAddress;
static bdaddr_t mouseAddress;

static char lastMouseInput[MOUSE_REPORT_SIZE] = {0};

static void check_for_control_command(int control_socket)
{
    unsigned char input[] = {0};
    unsigned char output[] = {0};
    struct timeval tv;

    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(control_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    
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

static void switch_current_client_to_index(tgot_ctx *ctx, int index)
{
    int previousSelectedClient = ctx->selected_client;
    tgot_client *currentClient = ctx->clients + ctx->selected_client;

    send(currentClient->interrupt, KEY_UP_SEQUENCE, sizeof(KEY_UP_SEQUENCE) - 1, 0);

    ctx->selected_client = (index >= 0 && index < ctx->clientCount) ? index : 0;
    
    // Close current client
    if (currentClient->connected && ctx->selected_client != previousSelectedClient)
    {
        close_client_connection(currentClient);
    }
}

static void switch_current_client(tgot_ctx *ctx)
{
    switch_current_client_to_index(ctx, ctx->selected_client + 1);
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

static void parseMouseInput(tgot_ctx *ctx, uint8_t *input, int bytesRead,
                            uint8_t *mouseReport, int len)
{
    int8_t x, y, xDiff, yDiff;

    x = ((input[16] << 4) & 0xf0) | ((input[15] >> 4) & 0x0f);
    y = input[17];
    
    xDiff = x - ctx->lastMouseX;
    yDiff = y - ctx->lastMouseY;
    
    mouseReport[0] = HID_PREFIX;
    mouseReport[1] = MOUSE_REPORT_ID;
    mouseReport[2] = input[2];
    mouseReport[3] = 0;
    mouseReport[8] = 0;
    mouseReport[9] = 0;
    
    memcpy(mouseReport + 4, input + 3, 4);
   
    // Check for released
    if (bytesRead != 23 || !(input[22] & 0x40))
    {
        if (ctx->mouseState == SCROLLING)
        {
            mouseReport[8] = yDiff;
            mouseReport[9] = xDiff;
        }
        
        ctx->mouseState = RELEASED;
        //printf("Released\n");
    }

    switch(ctx->mouseState)
    {
        case RELEASED:
        {
            if (bytesRead == 23 && (input[22] & 0x40))
            {
                ctx->mouseState = PRESSED;
                ctx->lastMouseX = x;
                ctx->lastMouseY = y;
                //printf("Pressed %d,%d\n", ctx->lastMouseX, ctx->lastMouseY);
            }
        }
        break;
        
        case PRESSED:
        {
            if (ctx->lastMouseX <= 16
                && ctx->lastMouseY >= -16
                && (abs(xDiff) > 2 || abs(yDiff) > 2))
            {
                //printf("Scroll zone\n");
                
                ctx->mouseState = SCROLLING;
                ctx->lastMouseX = x;
                ctx->lastMouseY = y;
            }

        }
        break;
        
        case SCROLLING:
        {
            if (abs(yDiff) >= 4)
            {
                ctx->lastMouseY = y;
                mouseReport[8] = yDiff;//> 0 ? 1 : -1;
            }

            if (abs(xDiff) >= 4)
            {
                ctx->lastMouseX = x;
                mouseReport[9] = xDiff;//> 0 ? 1 : -1;
            }
            //printf("Scrolling %d,%d\n", xDiff, yDiff);
        }
        break;

        default:
            break;
    }
}

static void bt_callback(char *input, int bytesRead, bdaddr_t *client,
                        channel_t channel, void *data)
{
    tgot_ctx *p_tgot_ctx = (tgot_ctx*)data;

    if (channel == INTERRUPT_CHANNEL)
    {
        if (memcmp(client, &keyboardAddress, sizeof(bdaddr_t)) == 0)
        {
            if (p_tgot_ctx->switchSeqOn)
            {
                switch_current_client_to_index(p_tgot_ctx, input[4] - 0x3a);
                p_tgot_ctx->switchSeqOn = false;
            }

            if (strncmp(input, SWITCH_SEQUENCE, bytesRead) == 0)
            {
                p_tgot_ctx->switchSeqOn = true;
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
            // Safety checks
            if (input[0] == HID_PREFIX && input[1] == 0x12
                && (input[2] & ~0x03) == 0)
            {
                uint8_t mouseReport[MOUSE_REPORT_SIZE];

                parseMouseInput(p_tgot_ctx, input, bytesRead, mouseReport,
                                MOUSE_REPORT_SIZE);

                if (memcmp(mouseReport, lastMouseInput, sizeof(lastMouseInput)) != 0)
                {
                    memcpy(lastMouseInput, mouseReport, sizeof(lastMouseInput));
                    send_to_client(p_tgot_ctx, mouseReport, MOUSE_REPORT_SIZE);
                }
            }
        }
    }
}

void bt_control_callback(int fd, bdaddr_t *address, void *data)
{
    char input[10];
    int batteryLevel;
    char *name = "Unknown";

    sleep(1);
    send(fd, "q", 1, 0); //Boot mode
    recv(fd, input, 10, 0);
    
    if (memcmp(address, &mouseAddress, sizeof(bdaddr_t)) == 0)
    {
        name = "Mouse";
        send(fd, "\x53\xFF\x90", 3, 0); //Select battery data
        recv(fd, input, 10, 0);
        send(fd, "\x41\xF0", 2, 0); //Get battery data
        recv(fd, input, 10, 0);

        batteryLevel = input[4];

        sleep(1);
        send(fd, "\x53\xF1\x02\x01", 4, 0);
        recv(fd, input, 10, 0);
    }
    else
    {
        name = "Keyboard";

        send(fd, "\x43\x47", 2, 0); //Get battery data
        recv(fd, input, 10, 0);

        batteryLevel = input[2];
    }

    printf("%s battery: %d%%\n", name, batteryLevel);
}

int main(int argc, char *argv[])
{
    tgot_client clients[] =
    {
        {"6c:40:08:a5:02:5d", -1, -1, false},
        {"60:BE:B5:30:61:AB", -1, -1, false},
        {"B8:27:EB:FC:19:77", -1, -1, false},
    };

    tgot_ctx ctx =
    {
        clients,
        0,
        ARRAY_SIZE(clients),
#ifdef APPLE_KEYBOARD
        false,
#endif
        RELEASED
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

