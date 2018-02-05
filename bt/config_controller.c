#include "mgmt_api/mgmt_api.h"
#include <dirent.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONTROLLER_PATH "/var/lib/bluetooth/%s"
#define LINK_KEYS_PATH CONTROLLER_PATH "/%s/info"

void loadLinkKeys(mgmt_api_ctx *ctx, const char *address)
{
    DIR *d;
    char path[128];
 
    sprintf(path, CONTROLLER_PATH, address);

    d = opendir(path);

    if (d)
    {
        int i = 0;
        struct link_key *linkKeys = NULL;
        struct dirent *dir;

        while ((dir = readdir(d)) != NULL)
        {
            GError *pError = NULL;
            GKeyFile *file = g_key_file_new();
            char *controllerFolder;

            sprintf(path, LINK_KEYS_PATH, address, dir->d_name);

            g_key_file_load_from_file(file, path, 0, &pError);

            if (pError == NULL)
            {
                gchar *key;
                gint keyType;
                gint pinLength;

                key = g_key_file_get_value(file, "LinkKey", "Key", &pError);

                if (pError)
                {
                    g_key_file_free(file);
                    continue;
                }

                keyType = g_key_file_get_integer(file, "LinkKey", "Type",
                        &pError);
                
                if (pError)
                {
                    g_key_file_free(file);
                    continue;
                }

                pinLength = g_key_file_get_integer(file, "LinkKey",
                                "PINLength", &pError);
                
                if (pError)
                {
                    g_key_file_free(file);
                    continue;
                }

                linkKeys = realloc(linkKeys, sizeof(struct link_key) * (i + 1));

                strcpy(linkKeys[i].address, dir->d_name);
                strcpy(linkKeys[i].key, key);

                g_free(key);

                linkKeys[i].key_type = keyType;
                linkKeys[i].pin_length = pinLength;
                i++;

                g_key_file_free(file);
            }
        }

        if (i > 0)
        {
            mgmt_api_set_link_keys(ctx, linkKeys, i);

            // Clean up
            free(linkKeys);
        }
    }
}

int main()
{
    char controller_address[BA_STR_LENGTH];

    mgmt_api_ctx *ctx = mgmt_api_connect();
    
    mgmt_api_set_power(ctx, false);
    mgmt_api_set_name(ctx, "Juanito");
    mgmt_api_set_class(ctx, 5, 0);
    mgmt_api_set_power(ctx, true);
    mgmt_api_set_connectable(ctx, true);
    mgmt_api_set_ssp(ctx, true);
    mgmt_api_set_bondable(ctx, true);

    mgmt_api_get_controller_address(ctx, controller_address);

    loadLinkKeys(ctx, controller_address);
    
    return 0;
}
