#include "bt/mgmt_api/mgmt_api.h"
#include <dirent.h>
#include <glib.h>

#define DEVICE_ADDRESS "00:1F:20:3D:28:C1"
#define LINK_KEY "339680868FE5E6A8E2A8A92FD013745C"

#define BA_STR_LENGTH (sizeof(bdaddr_t) * 3 - 1)
#define LINK_KEYS_PATH "/var/lib/bluetooth/%s/%s/info"

static void setLinkKey(mgmt_api_ctx *ctx, const char *address, const char *key,
        uint8_t keyType, uint8_t pinLength)
{
    bdaddr_t device_address;
    uint8_t bkey[16];
    int i;
    
    for (i = 0; i < 16; i++)
    {
        sscanf(key + i * 2, "%02x", bkey + i);
    }
    
    str2ba(address, &device_address);
    
    mgmt_api_set_link_key(ctx, &device_address, bkey, 5, 0);
}

void loadLinkKeys(mgmt_api_ctx *ctx, bdaddr_t *address)
{
    DIR *d;
    struct dirent *dir;
    char path[128];
    char str_address[BA_STR_LENGTH];

    ba2str(address, str_address);

    d = opendir(path);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            GError *pError = NULL;
            GKeyFile *file = g_key_file_new();
            char *controllerFolder;

            sprintf(path, LINK_KEYS_PATH, str_address, dir->d_name);

            g_key_file_load_from_file(file, path, 0, &pError);

            if (pError == NULL)
            {
                gchar *key;
                gint keyType;
                gint pinLength;

                key = g_key_file_get_value(file, "LinkKey", "Key", &pError);

                if (pError)
                {
                    continue;
                }

                keyType = g_key_file_get_integer(file, "LinkKey", "Type",
                        &pError);
                
                if (pError)
                {
                    continue;
                }

                pinLength = g_key_file_get_integer(file, "LinkKey",
                                "PINLength", &pError);
                
                if (pError)
                {
                    continue;
                }

                setLinkKey(ctx, dir->d_name, key, keyType, pinLength);

                g_key_file_free(file);
            }
        }
    }
}

int main()
{
    bdaddr_t controller_address;

    mgmt_api_ctx *ctx = mgmt_api_connect();
    
    mgmt_api_set_name(ctx, "Juanito");
    mgmt_api_set_class(ctx, 5, 0);
    mgmt_api_set_power(ctx, true);
    mgmt_api_set_connectable(ctx, true);

    mgmt_api_get_controller_address(ctx, &controller_address);

    loadLinkKeys(ctx, &controller_address);
    
    return 0;
}
