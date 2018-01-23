#include "bt/mgmt_api/mgmt_api.h"

int main()
{
    mgmt_api_ctx *ctx = mgmt_api_connect();
    
    mgmt_api_set_name(ctx, "Juanito");
    mgmt_api_set_class(ctx, 5, 0);
    mgmt_api_power(ctx, true);
    
    return 0;
}