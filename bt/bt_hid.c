#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "utils/bt_utils.h"
#include "bt_hid.h"

struct _bt_hid_ctx
{
    int control;
    int serverControl;
    int interrupt;
    int serverInterrupt;
};

bt_hid_ctx *bt_hid_init()
{
    bt_hid_ctx *ctx;
    
    ctx = malloc(sizeof(bt_hid_ctx));

    ctx->serverControl = -1;
    ctx->serverInterrupt = -1;
	
    ctx->control = bt_wait_for_connection_on_psm(&ctx->serverControl, 17);
	ctx->interrupt = bt_wait_for_connection_on_psm(&ctx->serverInterrupt, 19);


	return ctx;
}

int bt_hid_get_report(bt_hid_ctx *ctx, uint8_t *report, size_t size)
{
    int socket = ctx->interrupt;
	return read(socket, report, size);
}
