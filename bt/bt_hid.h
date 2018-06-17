typedef struct _bt_hid_ctx bt_hid_ctx;

typedef enum _channel_t
{
    CONTROL_CHANNEL,
    INTERRUPT_CHANNEL
} channel_t;

typedef void (*bt_callback_t)(char *input, int bytesRead, bdaddr_t *client,
                        channel_t channel, void *data);

typedef void (*bt_control_callback_t)(int fd, bdaddr_t *client, void *data);

bt_hid_ctx *bt_hid_init();
void bt_hid_main_loop(bt_hid_ctx*);
void bt_hid_register_callback(bt_hid_ctx*, bt_callback_t, void*);
void bt_hid_register_control_callback(bt_hid_ctx*, bt_control_callback_t,
                                      void*);
