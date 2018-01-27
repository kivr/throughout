#include <stdint.h>
#include <stdbool.h>
#include <bluetooth/bluetooth.h>

typedef int mgmt_api_ctx;

mgmt_api_ctx *mgmt_api_connect();

bdaddr_t *mgmt_api_get_controller_address();

void mgmt_api_set_power(mgmt_api_ctx*, bool);

void mgmt_api_set_connectable(mgmt_api_ctx*, bool);

void mgmt_api_set_name(mgmt_api_ctx*, const char*);

void mgmt_api_set_class(mgmt_api_ctx*, uint8_t major, uint8_t minor);

void mgmt_api_set_link_key(mgmt_api_ctx*, bdaddr_t *bdaddr, uint8_t val[16],
    uint8_t key_type, uint8_t pin_length);
