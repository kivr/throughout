#define HID_DESCRIPTOR \
    "\x05\x01"          /* Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x06"          /* Usage (Keyboard)*/ \
    "\xA1\x01"          /* Collection (Application)*/ \
    "\x85\x01"          /*   Report ID (1)*/ \
    "\x05\x07"          /*   Usage Page (Kbrd/Keypad)*/ \
    "\x19\xE0"          /*   Usage Minimum (0xE0)*/ \
    "\x29\xE7"          /*   Usage Maximum (0xE7)*/ \
    "\x15\x00"          /*   Logical Minimum (0)*/ \
    "\x25\x01"          /*   Logical Maximum (1)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x08"          /*   Report Count (8)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x75\x08"          /*   Report Size (8)*/ \
    "\x95\x01"          /*   Report Count (1)*/ \
    "\x81\x01"          /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x05"          /*   Report Count (5)*/ \
    "\x05\x08"          /*   Usage Page (LEDs)*/ \
    "\x19\x01"          /*   Usage Minimum (Num Lock)*/ \
    "\x29\x05"          /*   Usage Maximum (Kana)*/ \
    "\x91\x02"          /*   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\x75\x03"          /*   Report Size (3)*/ \
    "\x95\x01"          /*   Report Count (1)*/ \
    "\x91\x01"          /*   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\x75\x08"          /*   Report Size (8)*/ \
    "\x95\x06"          /*   Report Count (6)*/ \
    "\x15\x00"          /*   Logical Minimum (0)*/ \
    "\x26\xFF\x00"      /*   Logical Maximum (255)*/ \
    "\x05\x07"          /*   Usage Page (Kbrd/Keypad)*/ \
    "\x19\x00"          /*   Usage Minimum (0x00)*/ \
    "\x2A\xFF\x00"      /*   Usage Maximum (0xFF)*/ \
    "\x81\x00"          /*   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"              /* End Collection*/ \
    "\x05\x0C"          /* Usage Page (Consumer)*/ \
    "\x09\x01"          /* Usage (Consumer Control)*/ \
    "\xA1\x01"          /* Collection (Application)*/ \
    "\x85\x47"          /*   Report ID (71)*/ \
    "\x05\x01"          /*   Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x06"          /*   Usage (Keyboard)*/ \
    "\xA1\x02"          /*   Collection (Logical)*/ \
    "\x05\x06"          /*     Usage Page (Generic Dev Ctrls)*/ \
    "\x09\x20"          /*     Usage (Battery Strength)*/ \
    "\x15\x00"          /*     Logical Minimum (0)*/ \
    "\x26\xFF\x00"      /*     Logical Maximum (255)*/ \
    "\x75\x08"          /*     Report Size (8)*/ \
    "\x95\x01"          /*     Report Count (1)*/ \
    "\x81\x02"          /*     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"              /*   End Collection*/ \
    "\xC0"              /* End Collection*/ \
    "\x05\x0C"          /* Usage Page (Consumer)*/ \
    "\x09\x01"          /* Usage (Consumer Control)*/ \
    "\xA1\x01"          /* Collection (Application)*/ \
    "\x85\x11"          /*   Report ID (17)*/ \
    "\x15\x00"          /*   Logical Minimum (0)*/ \
    "\x25\x01"          /*   Logical Maximum (1)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x03"          /*   Report Count (3)*/ \
    "\x81\x01"          /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x01"          /*   Report Count (1)*/ \
    "\x05\x0C"          /*   Usage Page (Consumer)*/ \
    "\x09\xB8"          /*   Usage (Eject)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x06\xFF\x00"      /*   Usage Page (Reserved 0xFF)*/ \
    "\x09\x03"          /*   Usage (0x03)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x03"          /*   Report Count (3)*/ \
    "\x81\x01"          /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x05\x0C"          /*   Usage Page (Consumer)*/ \
    "\x85\x12"          /*   Report ID (18)*/ \
    "\x15\x00"          /*   Logical Minimum (0)*/ \
    "\x25\x01"          /*   Logical Maximum (1)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x01"          /*   Report Count (1)*/ \
    "\x09\xCD"          /*   Usage (Play/Pause)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x09\xB3"          /*   Usage (Fast Forward)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x09\xB4"          /*   Usage (Rewind)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x09\xB5"          /*   Usage (Scan Next Track)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x09\xB6"          /*   Usage (Scan Previous Track)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x81\x01"          /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x81\x01"          /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x81\x01"          /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x85\x13"          /*   Report ID (19)*/ \
    "\x15\x00"          /*   Logical Minimum (0)*/ \
    "\x25\x01"          /*   Logical Maximum (1)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x01"          /*   Report Count (1)*/ \
    "\x06\x01\xFF"      /*   Usage Page (Vendor Defined 0xFF01)*/ \
    "\x09\x0A"          /*   Usage (0x0A)*/ \
    "\x81\x02"          /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x06\x01\xFF"      /*   Usage Page (Vendor Defined 0xFF01)*/ \
    "\x09\x0C"          /*   Usage (0x0C)*/ \
    "\x81\x22"          /*   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)*/ \
    "\x75\x01"          /*   Report Size (1)*/ \
    "\x95\x06"          /*   Report Count (6)*/ \
    "\x81\x01"          /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x85\x09"          /*   Report ID (9)*/ \
    "\x09\x0B"          /*   Usage (0x0B)*/ \
    "\x75\x08"          /*   Report Size (8)*/ \
    "\x95\x01"          /*   Report Count (1)*/ \
    "\xB1\x02"          /*   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\x75\x08"          /*   Report Size (8)*/ \
    "\x95\x02"          /*   Report Count (2)*/ \
    "\xB1\x01"          /*   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\xC0"              /* End Collection*/ \
    "\x05\x01"     /* Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x02"     /* Usage (Mouse)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x14"     /*   Report ID (20)*/ \
    "\x09\x01"     /*   Usage (Pointer)*/ \
    "\xA1\x00"     /*   Collection (Physical)*/ \
    "\x05\x09"     /*     Usage Page (Button)*/ \
    "\x19\x01"     /*     Usage Minimum (0x01)*/ \
    "\x29\x10"     /*     Usage Maximum (0x10)*/ \
    "\x15\x00"     /*     Logical Minimum (0)*/ \
    "\x25\x01"     /*     Logical Maximum (1)*/ \
    "\x95\x10"     /*     Report Count (16)*/ \
    "\x75\x01"     /*     Report Size (1)*/ \
    "\x81\x02"     /*     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x05\x01"     /*     Usage Page (Generic Desktop Ctrls)*/ \
    "\x16\x01\x80" /*     Logical Minimum (-32767)*/ \
    "\x26\xFF\x7F" /*     Logical Maximum (32767)*/ \
    "\x75\x10"     /*     Report Size (16)*/ \
    "\x95\x02"     /*     Report Count (2)*/ \
    "\x09\x30"     /*     Usage (X)*/ \
    "\x09\x31"     /*     Usage (Y)*/ \
    "\x81\x06"     /*     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x15\x81"     /*     Logical Minimum (-127)*/ \
    "\x25\x7F"     /*     Logical Maximum (127)*/ \
    "\x75\x08"     /*     Report Size (8)*/ \
    "\x95\x01"     /*     Report Count (1)*/ \
    "\x09\x38"     /*     Usage (Wheel)*/ \
    "\x81\x06"     /*     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x05\x0C"     /*     Usage Page (Consumer)*/ \
    "\x0A\x38\x02" /*     Usage (AC Pan)*/ \
    "\x95\x01"     /*     Report Count (1)*/ \
    "\x81\x06"     /*     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"         /*   End Collection*/ \
    "\xC0"         /* End Collection*/

// 293 bytes

