/*
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

#ifndef RGB_AQUA_H_
#define RGB_AQUA_H_


#define LED_RED_PORT     2
#define SUPLA_PWM_COUNT  3
#define RGB_CONTROLLER_CHANNEL  0

// Red
#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define PWM_0_OUT_IO_NUM 13
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO13

// Green
#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_1_OUT_IO_NUM 14
#define PWM_1_OUT_IO_FUNC  FUNC_GPIO14

// Blue
#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_2_OUT_IO_NUM 5
#define PWM_2_OUT_IO_FUNC  FUNC_GPIO5

#define CFG_AFTER_GUID_GEN supla_esp_state.color[0] = 0x0FF00

void ICACHE_FLASH_ATTR supla_esp_board_pwm_init(void);
char ICACHE_FLASH_ATTR supla_esp_board_set_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness);
void ICACHE_FLASH_ATTR supla_esp_board_get_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness);
void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc);

#endif
