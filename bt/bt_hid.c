#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "utils/bt_utils.h"
#include "bt_hid.h"

#define BUFFER_SIZE 100

typedef struct _bt_hid_client
{
    bdaddr_t address;
    int control;
    int interrupt;
    bool controlCallbackCalled;
} bt_hid_client;

struct _bt_hid_ctx
{
    fd_set readSet;
    int serverControl;
    int serverInterrupt;
    int maxFD;
    bt_callback_t callback;
    void *callbackData;
    bt_control_callback_t controlCallback;
    void *controlCallbackData;
    int clientCount;
    bt_hid_client *clients;
};

static bt_hid_client *findClient(bt_hid_ctx *ctx, bdaddr_t *addr)
{
    int i;
    bt_hid_client *client = NULL;

    for (i = 0; i < ctx->clientCount; i++)
    {
        if (memcmp(&ctx->clients[i].address, addr, sizeof(bdaddr_t)) == 0)
        {
            client = &ctx->clients[i];
            break;
        }
    }

    return client;
}

static void updateClient(bt_hid_ctx *ctx, bdaddr_t *addr, int clientSocket,
                         channel_t channel)
{
    bt_hid_client *client = findClient(ctx, addr);

    if (client == NULL)
    {
        ctx->clientCount++;
        ctx->clients = realloc(ctx->clients,
                               sizeof(bt_hid_client) * ctx->clientCount);
        
        client = &ctx->clients[ctx->clientCount - 1];
        memcpy(&client->address, addr, sizeof(bdaddr_t));
        client->control = -1;
        client->interrupt = -1;
        client->controlCallbackCalled = false;
    }

    if (channel == CONTROL_CHANNEL)
    {
        client->control = clientSocket;
    }
    
    if (channel == INTERRUPT_CHANNEL)
    {
        client->interrupt = clientSocket;
    }

    // Call control callback when both channels are connected
    if (!client->controlCallbackCalled
        && ctx->controlCallback != NULL
        && client->control != -1
        && client->interrupt != -1)
    {
        client->controlCallbackCalled = true;
        ctx->controlCallback(client->control, &client->address,
                             ctx->controlCallbackData);
    }
    
    ctx->maxFD = clientSocket;
    FD_SET(clientSocket, &ctx->readSet);
}

static void acceptConnection(bt_hid_ctx *ctx, int serverSocket,
                             channel_t channel)
{
    struct sockaddr_l2 addr = {0};
    socklen_t len = sizeof(addr);
    int client;

    client = accept(serverSocket, (struct sockaddr*)&addr, &len);

    updateClient(ctx, &addr.l2_bdaddr, client, channel);
}

static void checkClients(bt_hid_ctx *ctx, fd_set *activeSet)
{
    int i;
    bt_hid_client *client;
    int bytesRead;
    char buffer[BUFFER_SIZE];

    for (i = 0, client = ctx->clients; i < ctx->clientCount; i++, client++)
    {
        if (client->control != -1
            && FD_ISSET(client->control, activeSet))
        {
            bytesRead = recv(client->control, buffer, BUFFER_SIZE, 0);
            ctx->callback(buffer, bytesRead, &client->address,
                          CONTROL_CHANNEL, ctx->callbackData);
        }

        if (client->interrupt != -1
            && FD_ISSET(client->interrupt, activeSet))
        {
            bytesRead = recv(client->interrupt, buffer, BUFFER_SIZE, 0);
            ctx->callback(buffer, bytesRead, &client->address,
                          INTERRUPT_CHANNEL, ctx->callbackData);
        }
    }
}

void bt_hid_main_loop(bt_hid_ctx *ctx)
{
    for (;;)
    {
        fd_set activeSet = ctx->readSet;
 
        select(ctx->maxFD + 1, &activeSet, NULL, NULL, NULL);

        if (FD_ISSET(ctx->serverControl, &activeSet))
        {
            acceptConnection(ctx, ctx->serverControl, CONTROL_CHANNEL);
        }
        
        if (FD_ISSET(ctx->serverInterrupt, &activeSet))
        {
            acceptConnection(ctx, ctx->serverInterrupt, INTERRUPT_CHANNEL);
        }
        
        checkClients(ctx, &activeSet);
    }
}

bt_hid_ctx *bt_hid_init()
{
    bt_hid_ctx *ctx = NULL;
    int controlFD, interruptFD;
    
    controlFD = bt_bind_on_psm(17);
    interruptFD = bt_bind_on_psm(19);
   
    if (controlFD != -1 && interruptFD != -1)
    { 
        ctx = calloc(1, sizeof(bt_hid_ctx));

        FD_ZERO(&ctx->readSet);
        
        ctx->serverControl = controlFD;
        ctx->serverInterrupt = interruptFD;
        
        FD_SET(controlFD, &ctx->readSet);
        FD_SET(interruptFD, &ctx->readSet);

        ctx->maxFD = interruptFD;
    }
	
    return ctx;
}

void bt_hid_register_callback(bt_hid_ctx *ctx, bt_callback_t callback,
                              void *callbackData)
{
    ctx->callback = callback;
    ctx->callbackData = callbackData;
}

void bt_hid_register_control_callback(bt_hid_ctx *ctx,
                                      bt_control_callback_t callback,
                                      void *data)
{
    ctx->controlCallback = callback;
    ctx->controlCallbackData = data;
}
