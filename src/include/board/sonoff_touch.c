/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#define B_RELAY1_PORT    12
#define B_CFG_PORT        0


const uint8_t rsa_public_key_bytes[512] = {
    0xc8, 0x8a, 0x22, 0xad, 0x80, 0x18, 0xb0, 0x7f,
    0xaa, 0x77, 0x3a, 0x6a, 0x68, 0xb5, 0xf2, 0x81,
    0xf4, 0x88, 0x7d, 0x50, 0x5c, 0x0f, 0x20, 0x18,
    0x71, 0x08, 0x8d, 0x95, 0x19, 0xb2, 0x8c, 0x33,
    0x3e, 0x2b, 0xa1, 0xc9, 0x16, 0x1c, 0x9b, 0xca,
    0x41, 0xb6, 0x3c, 0xc5, 0x35, 0xce, 0x55, 0x65,
    0xad, 0xc2, 0x57, 0x08, 0xf5, 0x3c, 0x64, 0xac,
    0x86, 0xf7, 0xac, 0x03, 0x34, 0x33, 0xca, 0xa3,
    0xce, 0xb6, 0x29, 0xcf, 0x7d, 0xe4, 0x3d, 0xd0,
    0x30, 0x10, 0xb1, 0x2e, 0x64, 0xc7, 0xb8, 0x70,
    0x7b, 0x0d, 0x15, 0x25, 0x86, 0xd9, 0xef, 0x58,
    0x56, 0x66, 0x14, 0x78, 0xd8, 0x5d, 0xa0, 0x69,
    0x49, 0xb4, 0x86, 0x45, 0xc2, 0xf6, 0x2c, 0x2b,
    0x93, 0x01, 0xed, 0x86, 0x89, 0xe0, 0x5c, 0xd5,
    0x05, 0xcf, 0x7d, 0x3a, 0x3b, 0xd6, 0xe8, 0xde,
    0x8e, 0xae, 0x06, 0x30, 0x45, 0x42, 0x7f, 0x60,
    0x8a, 0x59, 0xf7, 0xd1, 0x1a, 0x08, 0x7a, 0x79,
    0xbe, 0x70, 0x0f, 0xcb, 0xaa, 0x92, 0x5b, 0x5e,
    0x86, 0x63, 0xf1, 0xfd, 0x15, 0x85, 0x1c, 0xf6,
    0x09, 0x0a, 0xa1, 0x7a, 0xb4, 0x64, 0x67, 0x77,
    0x3a, 0xe0, 0x6e, 0x70, 0xd2, 0x1a, 0xe5, 0x33,
    0x23, 0xc9, 0xb9, 0xd8, 0x91, 0xff, 0xdb, 0x18,
    0x33, 0x9a, 0x6f, 0x78, 0x41, 0x30, 0x79, 0xc4,
    0xeb, 0xc2, 0xa8, 0x69, 0xc2, 0x4d, 0x1a, 0xfb,
    0xfc, 0x67, 0x9d, 0x78, 0xe1, 0x30, 0xa8, 0xb2,
    0x4d, 0xc6, 0x7d, 0x31, 0xaa, 0x0e, 0x03, 0xe9,
    0xb7, 0x77, 0x36, 0x1e, 0x76, 0xbf, 0xb8, 0x9d,
    0xee, 0x27, 0x11, 0x3c, 0x5c, 0xcb, 0xb6, 0x66,
    0xb6, 0xce, 0x81, 0xa2, 0xe1, 0x2f, 0xe8, 0x5f,
    0xb7, 0x62, 0xf0, 0x90, 0x80, 0xa3, 0xdb, 0x6c,
    0x7e, 0x23, 0x46, 0x5e, 0xa3, 0x1a, 0x82, 0x2b,
    0xcb, 0x62, 0x1a, 0xd7, 0x8f, 0xf2, 0x5a, 0x07,
    0xd9, 0xba, 0x43, 0xcc, 0x6b, 0x0d, 0xec, 0xe9,
    0x1e, 0x1e, 0x88, 0x69, 0xc1, 0x19, 0xa1, 0xf4,
    0x10, 0x80, 0x14, 0xca, 0x2f, 0x0a, 0x40, 0x5a,
    0xdd, 0x2a, 0xc9, 0x09, 0x5d, 0xc4, 0xde, 0xb4,
    0x2e, 0x95, 0x34, 0x58, 0x03, 0x27, 0x37, 0x34,
    0x12, 0x13, 0xe2, 0xfe, 0x00, 0x78, 0x28, 0xd2,
    0x85, 0x9a, 0x69, 0xfa, 0x49, 0x7b, 0x80, 0x5c,
    0x2f, 0x23, 0x32, 0xce, 0x82, 0xf4, 0xb2, 0x5c,
    0x45, 0xa3, 0xce, 0x01, 0x4d, 0x69, 0x24, 0xd8,
    0x20, 0xff, 0xa3, 0x17, 0x84, 0xb2, 0x08, 0x0a,
    0x53, 0xde, 0x5f, 0xea, 0x98, 0xf8, 0x6e, 0x74,
    0xa5, 0x31, 0x54, 0xd9, 0x03, 0x20, 0xb3, 0x9f,
    0xd6, 0x86, 0xf7, 0xf0, 0x87, 0x6f, 0x97, 0x28,
    0x11, 0xc0, 0x0b, 0xd9, 0x72, 0xe5, 0xcd, 0x2b,
    0x59, 0x52, 0xb5, 0xff, 0x68, 0xa6, 0xca, 0x1c,
    0xac, 0xa0, 0xab, 0x31, 0x84, 0x9b, 0x84, 0x8c,
    0x25, 0x2a, 0x32, 0x4a, 0xf4, 0x57, 0xea, 0xb2,
    0xf5, 0x46, 0x63, 0x9f, 0xd0, 0xc5, 0x09, 0xc4,
    0x74, 0x82, 0x2c, 0x96, 0x6c, 0x12, 0x2d, 0x14,
    0x0d, 0xe7, 0x55, 0xda, 0x85, 0xcf, 0x59, 0x57,
    0xa4, 0xcf, 0x8a, 0x54, 0x7b, 0x29, 0xcf, 0xe2,
    0x47, 0xdd, 0x15, 0x34, 0xb3, 0xd3, 0xac, 0x5a,
    0x66, 0x75, 0x91, 0x26, 0x70, 0xbf, 0x5d, 0x41,
    0x32, 0xe1, 0x7f, 0x3c, 0x4b, 0x55, 0xb1, 0x17,
    0x1c, 0x8a, 0x2b, 0x93, 0xfd, 0xcc, 0x72, 0x5d,
    0xec, 0x6c, 0x71, 0x42, 0x22, 0x02, 0x30, 0x3b,
    0x6d, 0x25, 0x35, 0xff, 0xb5, 0x8a, 0x47, 0xf5,
    0x31, 0x43, 0x61, 0x5b, 0x36, 0x43, 0xc3, 0x47,
    0x0c, 0xde, 0x66, 0xa4, 0x73, 0x3c, 0x56, 0xbb,
    0xb2, 0xb0, 0x3b, 0x99, 0x09, 0x41, 0xfb, 0x56,
    0xca, 0x89, 0xa4, 0x62, 0x36, 0xb6, 0x3d, 0xbb,
    0x72, 0x3f, 0x50, 0x82, 0x57, 0x21, 0xbc, 0xb3
};

void supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {

		ets_snprintf(buffer, buffer_size, "SONOFF-TOUCH");

}


void supla_esp_board_gpio_init(void) {
		
	supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
	supla_input_cfg[0].gpio_id = B_CFG_PORT;
	supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
	supla_input_cfg[0].relay_gpio_id = B_RELAY1_PORT;
	supla_input_cfg[0].channel = 0;

	// ---------------------------------------
	// ---------------------------------------

    supla_relay_cfg[0].gpio_id = B_RELAY1_PORT;
    supla_relay_cfg[0].flags = RELAY_FLAG_RESTORE_FORCE;
    supla_relay_cfg[0].channel = 0;
    

}

void supla_esp_board_set_channels(TDS_SuplaDeviceChannel_B *channels, unsigned char *channel_count) {
	
	*channel_count = 2;

	channels[0].Number = 0;
	channels[0].Type = SUPLA_CHANNELTYPE_RELAY;

	channels[0].FuncList = SUPLA_BIT_RELAYFUNC_POWERSWITCH \
								| SUPLA_BIT_RELAYFUNC_LIGHTSWITCH;

	channels[0].Default = SUPLA_CHANNELFNC_LIGHTSWITCH;

	channels[0].value[0] = supla_esp_gpio_relay_on(B_RELAY1_PORT);

	channels[1].Number = 1;
	channels[1].Type = SUPLA_CHANNELTYPE_THERMOMETERDS18B20;

	channels[1].FuncList = 0;
	channels[1].Default = 0;

	supla_get_temperature(channels[1].value);

}

void supla_esp_board_send_channel_values_with_delay(void *srpc) {

	supla_esp_channel_value_changed(0, supla_esp_gpio_relay_on(B_RELAY1_PORT));

}
