typedef struct _usb_hid_ctx usb_hid_ctx;

usb_hid_ctx *usb_hid_init(uint16_t vid, uint16_t pid, uint16_t interface);

void usb_hid_exit(usb_hid_ctx *ctx);

int usb_hid_get_report(usb_hid_ctx *ctx,
		uint16_t endpoint, uint8_t *report, int size);
