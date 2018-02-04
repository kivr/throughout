typedef struct _bt_hid_ctx bt_hid_ctx;

bt_hid_ctx *bt_hid_init();
int bt_hid_get_report(bt_hid_ctx*, uint8_t *report, size_t size);
