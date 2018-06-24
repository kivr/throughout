#define HID_DESCRIPTOR \
    "\x05\x01"     /* Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x06"     /* Usage (Keyboard)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x01"     /*   Report ID (1)*/ \
    "\x75\x01"     /*   Report Size (1)*/ \
    "\x95\x08"     /*   Report Count (8)*/ \
    "\x05\x07"     /*   Usage Page (Kbrd/Keypad)*/ \
    "\x19\xE0"     /*   Usage Minimum (0xE0)*/ \
    "\x29\xE7"     /*   Usage Maximum (0xE7)*/ \
    "\x15\x00"     /*   Logical Minimum (0)*/ \
    "\x25\x01"     /*   Logical Maximum (1)*/ \
    "\x81\x02"     /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x95\x01"     /*   Report Count (1)*/ \
    "\x75\x08"     /*   Report Size (8)*/ \
    "\x81\x03"     /*   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x95\x05"     /*   Report Count (5)*/ \
    "\x75\x01"     /*   Report Size (1)*/ \
    "\x05\x08"     /*   Usage Page (LEDs)*/ \
    "\x19\x01"     /*   Usage Minimum (Num Lock)*/ \
    "\x29\x05"     /*   Usage Maximum (Kana)*/ \
    "\x91\x02"     /*   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\x95\x01"     /*   Report Count (1)*/ \
    "\x75\x03"     /*   Report Size (3)*/ \
    "\x91\x03"     /*   Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\x95\x06"     /*   Report Count (6)*/ \
    "\x75\x08"     /*   Report Size (8)*/ \
    "\x15\x00"     /*   Logical Minimum (0)*/ \
    "\x26\xFF\x00" /*   Logical Maximum (255)*/ \
    "\x05\x07"     /*   Usage Page (Kbrd/Keypad)*/ \
    "\x19\x00"     /*   Usage Minimum (0x00)*/ \
    "\x29\xFF"     /*   Usage Maximum (0xFF)*/ \
    "\x81\x00"     /*   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"         /* End Collection*/ \
    "\x05\x0C"     /* Usage Page (Consumer)*/ \
    "\x09\x01"     /* Usage (Consumer Control)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x02"     /*   Report ID (2)*/ \
    "\x15\x00"     /*   Logical Minimum (0)*/ \
    "\x25\x01"     /*   Logical Maximum (1)*/ \
    "\x75\x01"     /*   Report Size (1)*/ \
    "\x95\x12"     /*   Report Count (18)*/ \
    "\x0A\x94\x01" /*   Usage (AL Local Machine Browser)*/ \
    "\x0A\x92\x01" /*   Usage (AL Calculator)*/ \
    "\x0A\x83\x01" /*   Usage (AL Consumer Control Configuration)*/ \
    "\x0A\x23\x02" /*   Usage (AC Home)*/ \
    "\x0A\x8A\x01" /*   Usage (AL Email Reader)*/ \
    "\x0A\x82\x01" /*   Usage (AL Programmable Button Configuration)*/ \
    "\x0A\x21\x02" /*   Usage (AC Search)*/ \
    "\x0A\x30\x00" /*   Usage (Power)*/ \
    "\x0A\x25\x02" /*   Usage (AC Forward)*/ \
    "\x0A\x23\x02" /*   Usage (AC Home)*/ \
    "\x0A\x27\x02" /*   Usage (AC Refresh)*/ \
    "\x09\xB6"     /*   Usage (Scan Previous Track)*/ \
    "\x09\xB5"     /*   Usage (Scan Next Track)*/ \
    "\x09\xB8"     /*   Usage (Eject)*/ \
    "\x09\xCD"     /*   Usage (Play/Pause)*/ \
    "\x09\xE9"     /*   Usage (Volume Increment)*/ \
    "\x09\xEA"     /*   Usage (Volume Decrement)*/ \
    "\x09\xE2"     /*   Usage (Mute)*/ \
    "\x81\x02"     /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x95\x01"     /*   Report Count (1)*/ \
    "\x75\x06"     /*   Report Size (6)*/ \
    "\x81\x03"     /*   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"         /* End Collection*/ \
    "\x05\x0C"     /* Usage Page (Consumer)*/ \
    "\x09\x01"     /* Usage (Consumer Control)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x03"     /*   Report ID (3)*/ \
    "\x05\x01"     /*   Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x06"     /*   Usage (Keyboard)*/ \
    "\xA1\x02"     /*   Collection (Logical)*/ \
    "\x05\x06"     /*     Usage Page (Generic Dev Ctrls)*/ \
    "\x09\x20"     /*     Usage (Battery Strength)*/ \
    "\x15\x00"     /*     Logical Minimum (0)*/ \
    "\x26\xFF\x00" /*     Logical Maximum (255)*/ \
    "\x75\x08"     /*     Report Size (8)*/ \
    "\x95\x01"     /*     Report Count (1)*/ \
    "\x81\x02"     /*     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"         /*   End Collection*/ \
    "\xC0"         /* End Collection*/ \
    "\x05\x01"     /* Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x80"     /* Usage (Sys Control)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x04"     /*   Report ID (4)*/ \
    "\x15\x00"     /*   Logical Minimum (0)*/ \
    "\x25\x01"     /*   Logical Maximum (1)*/ \
    "\x75\x01"     /*   Report Size (1)*/ \
    "\x95\x01"     /*   Report Count (1)*/ \
    "\x09\x82"     /*   Usage (Sys Sleep)*/ \
    "\x81\x02"     /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x95\x01"     /*   Report Count (1)*/ \
    "\x75\x07"     /*   Report Size (7)*/ \
    "\x81\x03"     /*   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"         /* End Collection*/ \
    "\x05\x0C"     /* Usage Page (Consumer)*/ \
    "\x09\x01"     /* Usage (Consumer Control)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x05"     /*   Report ID (5)*/ \
    "\x05\x01"     /*   Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x06"     /*   Usage (Keyboard)*/ \
    "\xA1\x02"     /*   Collection (Logical)*/ \
    "\x06\x00\xFF" /*     Usage Page (Vendor Defined 0xFF00)*/ \
    "\x25\x01"     /*     Logical Maximum (1)*/ \
    "\x75\x01"     /*     Report Size (1)*/ \
    "\x95\x02"     /*     Report Count (2)*/ \
    "\x0A\x03\xFE" /*     Usage (0xFE03)*/ \
    "\x0A\x04\xFE" /*     Usage (0xFE04)*/ \
    "\x81\x02"     /*     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x95\x06"     /*     Report Count (6)*/ \
    "\x81\x03"     /*     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"         /*   End Collection*/ \
    "\xC0"         /* End Collection*/ \
    "\x05\x0C"     /* Usage Page (Consumer)*/ \
    "\x09\x01"     /* Usage (Consumer Control)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\xFF"     /*   Report ID (-1)*/ \
    "\x05\x06"     /*   Usage Page (Generic Dev Ctrls)*/ \
    "\x95\x01"     /*   Report Count (1)*/ \
    "\x75\x02"     /*   Report Size (2)*/ \
    "\x19\x24"     /*   Usage Minimum (0x24)*/ \
    "\x29\x26"     /*   Usage Maximum (0x26)*/ \
    "\x81\x02"     /*   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x75\x06"     /*   Report Size (6)*/ \
    "\x81\x01"     /*   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\xC0"         /* End Collection*/ \
    "\x06\x00\xFF" /* Usage Page (Vendor Defined 0xFF00)*/ \
    "\x09\x01"     /* Usage (0x01)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x10"     /*   Report ID (16)*/ \
    "\x75\x08"     /*   Report Size (8)*/ \
    "\x95\x06"     /*   Report Count (6)*/ \
    "\x15\x00"     /*   Logical Minimum (0)*/ \
    "\x26\xFF\x00" /*   Logical Maximum (255)*/ \
    "\x09\x01"     /*   Usage (0x01)*/ \
    "\x81\x00"     /*   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x09\x01"     /*   Usage (0x01)*/ \
    "\x91\x00"     /*   Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\xC0"         /* End Collection*/ \
    "\x06\x00\xFF" /* Usage Page (Vendor Defined 0xFF00)*/ \
    "\x09\x02"     /* Usage (0x02)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x11"     /*   Report ID (17)*/ \
    "\x75\x08"     /*   Report Size (8)*/ \
    "\x95\x13"     /*   Report Count (19)*/ \
    "\x15\x00"     /*   Logical Minimum (0)*/ \
    "\x26\xFF\x00" /*   Logical Maximum (255)*/ \
    "\x09\x02"     /*   Usage (0x02)*/ \
    "\x81\x00"     /*   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)*/ \
    "\x09\x02"     /*   Usage (0x02)*/ \
    "\x91\x00"     /*   Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)*/ \
    "\xC0"         /* End Collection*/ \
    "\x05\x01"     /* Usage Page (Generic Desktop Ctrls)*/ \
    "\x09\x02"     /* Usage (Mouse)*/ \
    "\xA1\x01"     /* Collection (Application)*/ \
    "\x85\x12"     /*   Report ID (18)*/ \
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

// 381 bytes

