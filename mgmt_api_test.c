#include "bt/mgmt_api/mgmt_api.h"

#define DEVICE_ADDRESS "00:1F:20:3D:28:C1"
#define LINK_KEY "339680868FE5E6A8E2A8A92FD013745C"

static void setLinkKeys(mgmt_api_ctx *ctx)
{
    bdaddr_t device_address;
    uint8_t key[16];
    int i;
    
    for (i = 0; i < 16; i++)
    {
        sscanf(LINK_KEY + i * 2, "%02x", key + i);
    }
    
    str2ba(DEVICE_ADDRESS, &device_address);
    
    mgmt_api_set_link_key(ctx, &device_address, key, 5, 0);
}

int main()
{
    mgmt_api_ctx *ctx = mgmt_api_connect();
    
    mgmt_api_set_name(ctx, "Juanito");
    mgmt_api_set_class(ctx, 5, 0);
    mgmt_api_set_power(ctx, true);
    mgmt_api_set_connectable(ctx, true);

    setLinkKeys(ctx);
    
    return 0;
}