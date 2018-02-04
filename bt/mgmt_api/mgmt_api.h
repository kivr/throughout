#include <stdint.h>
#include <stdbool.h>

#define BA_STR_LENGTH 18
#define BA_KEY_LENGTH 16

struct link_key
{
    char address[BA_STR_LENGTH];
    char key[BA_KEY_LENGTH * 2 + 1]; // 2 hex digits per byte plus terminator
    uint8_t key_type;
    int pin_length;
};

typedef int mgmt_api_ctx;

mgmt_api_ctx *mgmt_api_connect();

void mgmt_api_get_controller_address(mgmt_api_ctx*, char[BA_STR_LENGTH]);

void mgmt_api_set_power(mgmt_api_ctx*, bool);

void mgmt_api_set_ssp(mgmt_api_ctx*, bool);

void mgmt_api_set_bondable(mgmt_api_ctx*, bool);

void mgmt_api_set_connectable(mgmt_api_ctx*, bool);

void mgmt_api_set_name(mgmt_api_ctx*, const char*);

void mgmt_api_set_class(mgmt_api_ctx*, uint8_t major, uint8_t minor);

void mgmt_api_set_link_keys(mgmt_api_ctx*, struct link_key*, uint16_t size);
