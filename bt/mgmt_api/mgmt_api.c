#include "mgmt_api.h"
#include <bluetooth/bluetooth.h>
#include "mgmt.h"
#include <sys/socket.h>
#include <sys/uio.h>
#include <bluetooth/hci.h>
#include <stdlib.h>
#include <unistd.h>

static void emptyEventQueue(mgmt_api_ctx *ctx)
{
    char buffer[64];
    while (read(*ctx, buffer, sizeof(buffer)) != -1);
}

mgmt_api_ctx *mgmt_api_connect()
{
    mgmt_api_ctx *result = NULL;

    int s = socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC | SOCK_NONBLOCK,
             BTPROTO_HCI);
    if (s >= 0)
    {
        struct sockaddr_hci addr;
        
        memset(&addr, 0, sizeof(addr));
        addr.hci_family = AF_BLUETOOTH;
        addr.hci_dev = HCI_DEV_NONE;
        addr.hci_channel = HCI_CHANNEL_CONTROL;

        if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) >= 0)
        {
            result = (mgmt_api_ctx*)malloc(sizeof(mgmt_api_ctx));
            if (result)
            {
                *result = s;
            }
        }
    }

    return result;
}

void mgmt_api_get_controller_address(mgmt_api_ctx *ctx,
		char *controller_address)
{
    //Output
    struct mgmt_hdr header;

    struct iovec iov[3];

    //Input
    struct mgmt_ev_cmd_complete cmd_complete;
    struct mgmt_rp_read_info info;

    emptyEventQueue(ctx);

    header.opcode = MGMT_OP_READ_INFO;
    header.index = 0;
    header.len = 0;

    write(*ctx, &header, sizeof(header));

    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &cmd_complete;
    iov[1].iov_len = sizeof(cmd_complete);
    iov[2].iov_base = &info;
    iov[2].iov_len = sizeof(info);

    readv(*ctx, iov, 3);

    ba2str(&info.bdaddr, controller_address);
}

void mgmt_api_set_power(mgmt_api_ctx *ctx, bool powerState)
{
    struct mgmt_hdr header;
    uint8_t value;
    struct iovec iov[2];
    
    header.opcode = MGMT_OP_SET_POWERED;
    header.index = 0;
    header.len = sizeof(value);
    
    value = powerState ? 1 : 0;
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &value;
    iov[1].iov_len = sizeof(value);
    
    writev(*ctx, iov, 2);
}

void mgmt_api_set_ssp(mgmt_api_ctx *ctx, bool ssp)
{
    struct mgmt_hdr header;
    uint8_t value;
    struct iovec iov[2];
    
    header.opcode = MGMT_OP_SET_SSP;
    header.index = 0;
    header.len = sizeof(value);
    
    value = ssp ? 1 : 0;
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &value;
    iov[1].iov_len = sizeof(value);
    
    writev(*ctx, iov, 2);
}

void mgmt_api_set_bondable(mgmt_api_ctx *ctx, bool bondable)
{
    struct mgmt_hdr header;
    uint8_t value;
    struct iovec iov[2];
    
    header.opcode = MGMT_OP_SET_BONDABLE;
    header.index = 0;
    header.len = sizeof(value);
    
    value = bondable ? 1 : 0;
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &value;
    iov[1].iov_len = sizeof(value);
    
    writev(*ctx, iov, 2);
}

void mgmt_api_set_connectable(mgmt_api_ctx *ctx, bool connectable)
{
    struct mgmt_hdr header;
    uint8_t value;
    struct iovec iov[2];
    
    header.opcode = MGMT_OP_SET_CONNECTABLE;
    header.index = 0;
    header.len = sizeof(value);
    
    value = connectable ? 1 : 0;
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &value;
    iov[1].iov_len = sizeof(value);
    
    writev(*ctx, iov, 2);
}

void mgmt_api_set_name(mgmt_api_ctx *ctx, const char *in_name)
{
    struct mgmt_hdr header;
    struct mgmt_cp_set_local_name name;
    struct iovec iov[2];

    if (strlen(in_name) >= MGMT_MAX_SHORT_NAME_LENGTH)
    {
        return;
    }
    
    header.opcode = MGMT_OP_SET_LOCAL_NAME;
    header.index = 0;
    header.len = sizeof(name);
    
    memset(&name, 0, sizeof(name));
    
    strcpy(name.name, in_name);
    strcpy(name.short_name, in_name);
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &name;
    iov[1].iov_len = sizeof(name);
    
    writev(*ctx, iov, 2);
}

void mgmt_api_set_class(mgmt_api_ctx *ctx, uint8_t major, uint8_t minor)
{
    struct mgmt_hdr header;
    struct mgmt_cp_set_dev_class dev_class;
    struct iovec iov[2];
    
    header.opcode = MGMT_OP_SET_DEV_CLASS;
    header.index = 0;
    header.len = sizeof(dev_class);
    
    dev_class.major = major;
    dev_class.minor = minor;
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &dev_class;
    iov[1].iov_len = sizeof(dev_class);
    
    writev(*ctx, iov, 2);
}

static void keyStringToBytes(const char *s_key, uint8_t key[BA_KEY_LENGTH])
{
    int i;
    
    for (i = 0; i < BA_KEY_LENGTH; i++)
    {
        sscanf(s_key + i * 2, "%02x", key + i);
    } 
}

void mgmt_api_set_link_keys(mgmt_api_ctx *ctx, struct link_key *s_link_keys,
    uint16_t size)
{
    int i;
    struct mgmt_hdr header;
    struct mgmt_cp_load_link_keys *linkKeys;
    struct iovec iov[2];

    int dataSize = sizeof(struct mgmt_cp_load_link_keys) +
        sizeof(struct mgmt_link_key_info) * size;
    
    header.opcode = MGMT_OP_LOAD_LINK_KEYS;
    header.index = 0;
    header.len = dataSize;

    linkKeys = malloc(dataSize);   
 
    linkKeys->debug_keys = 0;
    linkKeys->key_count = size;

    for (i = 0; i < size; i++)
    {
        str2ba(s_link_keys[i].address, &(linkKeys->keys[i].addr.bdaddr));
        linkKeys->keys[i].addr.type = 0;
        linkKeys->keys[i].type = s_link_keys[i].key_type;
        keyStringToBytes(s_link_keys[i].key, linkKeys->keys[i].val);
        linkKeys->keys[i].pin_len = s_link_keys[i].pin_length;
    }
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = linkKeys;
    iov[1].iov_len = dataSize;
    
    writev(*ctx, iov, 2);
}
