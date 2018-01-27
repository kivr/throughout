#include "mgmt_api.h"
#include "mgmt.h"
#include <sys/socket.h>
#include <sys/uio.h>
#include <bluetooth/hci.h>
#include <stdlib.h>

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

bdaddr_t *mgmt_api_get_controller_address()
{
    struct mgmt_hdr header;
    struct mgmt_rp_read_info info;
	return NULL;    
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

void mgmt_api_set_name(mgmt_api_ctx *ctx, const char *name)
{
    struct mgmt_hdr header;
    struct mgmt_cp_set_local_name localName;
    struct iovec iov[2];
    
    if (strlen(name) >= MGMT_MAX_SHORT_NAME_LENGTH)
    {
        return;
    }
    
    header.opcode = MGMT_OP_SET_LOCAL_NAME;
    header.index = 0;
    header.len = sizeof(localName);
    
    memset(&localName, 0, sizeof(localName));
    
    strcpy(localName.name, name);
    strcpy(localName.short_name, name);
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &localName;
    iov[1].iov_len = sizeof(localName);
    
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

void mgmt_api_set_link_key(mgmt_api_ctx *ctx, bdaddr_t *bdaddr, uint8_t val[16],
    uint8_t key_type, uint8_t pin_length)
{
    struct mgmt_hdr header;
    struct mgmt_cp_load_link_keys linkKeys;
    struct mgmt_link_key_info keyInfo;
    struct iovec iov[3];
    
    header.opcode = MGMT_OP_LOAD_LINK_KEYS;
    header.index = 0;
    header.len = sizeof(linkKeys) + sizeof(struct mgmt_link_key_info);
    
    linkKeys.debug_keys = 0;
    linkKeys.key_count = 1;
    
    keyInfo.addr.bdaddr = *bdaddr;
    keyInfo.addr.type = 0;
    keyInfo.type = key_type;
    memcpy(keyInfo.val, val, 16);
    keyInfo.pin_len = pin_length;
    
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &linkKeys;
    iov[1].iov_len = sizeof(linkKeys);
    iov[2].iov_base = &keyInfo;
    iov[2].iov_len = sizeof(keyInfo);
    
    writev(*ctx, iov, 3);
}
