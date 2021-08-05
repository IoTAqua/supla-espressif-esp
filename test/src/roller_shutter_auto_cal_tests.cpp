/* Copyright (C) AC SOFTWARE SP. Z O.O.

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

#include <eagle_soc_mock.h>
#include <eagle_soc_stub.h>
#include <gtest/gtest.h>
#include <time_mock.h>

extern "C" {
#include "board_stub.h"
#include <osapi.h>
#include <supla_esp_cfg.h>
#include <supla_esp_gpio.h>
#include <supla_esp_devconn.h>
}

#define RS_DIRECTION_NONE 0
#define RS_DIRECTION_UP 2
#define RS_DIRECTION_DOWN 1

#define UP_GPIO 1
#define DOWN_GPIO 2

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnPointee;
using ::testing::SaveArg;

// method will be called by supla_esp_gpio_init method in order to initialize
// gpio input/outputs board configuration (supla_esb_board_gpio_init)
void gpioCallback2() {
  // up
  supla_relay_cfg[0].gpio_id = UP_GPIO;
  supla_relay_cfg[0].channel = 0;
  supla_relay_cfg[0].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  // down
  supla_relay_cfg[1].gpio_id = DOWN_GPIO;
  supla_relay_cfg[1].channel = 0;
  supla_relay_cfg[1].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  supla_rs_cfg[0].up = &supla_relay_cfg[0];
  supla_rs_cfg[0].down = &supla_relay_cfg[1];
  supla_rs_cfg[0].delayed_trigger.value = 0;
}

class RollerShutterAutoCalF : public ::testing::Test {
public:
  TimeMock time;
  EagleSocStub eagleStub;

  void SetUp() override {
    upTime = 1100;
    downTime = 1200;
    memset(&supla_esp_cfg, 0, sizeof(supla_esp_cfg));
    memset(&supla_esp_state, 0, sizeof(SuplaEspState));
    cleanupTimers();
    supla_esp_gpio_init_time = 0;
    gpioInitCb = *gpioCallback2;
  }

  void TearDown() override {
    upTime = 1100;
    downTime = 1200;
    memset(&supla_esp_cfg, 0, sizeof(supla_esp_cfg));
    memset(&supla_esp_state, 0, sizeof(SuplaEspState));
    supla_esp_gpio_init_time = 0;
    cleanupTimers();

    gpioInitCb = nullptr;
  }
};

// method will be called by supla_esp_gpio_init method in order to initialize
// gpio input/outputs board configuration (supla_esb_board_gpio_init)
void gpioCallbackFourRs() {
  // First RS with autocal
  // up
  supla_relay_cfg[0].gpio_id = 1;
  supla_relay_cfg[0].channel = 0;
  supla_relay_cfg[0].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  // down
  supla_relay_cfg[1].gpio_id = 2;
  supla_relay_cfg[1].channel = 0;
  supla_relay_cfg[1].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  supla_rs_cfg[0].up = &supla_relay_cfg[0];
  supla_rs_cfg[0].down = &supla_relay_cfg[1];
  supla_rs_cfg[0].delayed_trigger.value = 0;

  // Second RS with autocal
  // up
  supla_relay_cfg[2].gpio_id = 3;
  supla_relay_cfg[2].channel = 1;
  supla_relay_cfg[2].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  // down
  supla_relay_cfg[3].gpio_id = 4;
  supla_relay_cfg[3].channel = 1;
  supla_relay_cfg[3].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  supla_rs_cfg[1].up = &supla_relay_cfg[2];
  supla_rs_cfg[1].down = &supla_relay_cfg[3];
  supla_rs_cfg[1].delayed_trigger.value = 0;

  // Third RS without autocal
  // up
  supla_relay_cfg[4].gpio_id = 5;
  supla_relay_cfg[4].channel = 2;
  supla_relay_cfg[4].channel_flags = 0;

  // down
  supla_relay_cfg[5].gpio_id = 6;
  supla_relay_cfg[5].channel = 2;
  supla_relay_cfg[5].channel_flags = 0;

  supla_rs_cfg[2].up = &supla_relay_cfg[4];
  supla_rs_cfg[2].down = &supla_relay_cfg[5];
  supla_rs_cfg[2].delayed_trigger.value = 0;

  // Fourth RS with autocal
  // up
  supla_relay_cfg[6].gpio_id = 7;
  supla_relay_cfg[6].channel = 3;
  supla_relay_cfg[6].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  // down
  supla_relay_cfg[7].gpio_id = 8;
  supla_relay_cfg[7].channel = 3;
  supla_relay_cfg[7].channel_flags = SUPLA_CHANNEL_FLAG_RS_AUTO_CALIBRATION;

  supla_rs_cfg[3].up = &supla_relay_cfg[6];
  supla_rs_cfg[3].down = &supla_relay_cfg[7];
  supla_rs_cfg[3].delayed_trigger.value = 0;
}

class RollerShutterFourAutoCalF : public ::testing::Test {
public:
  TimeMock time;
  EagleSocStub eagleStub;

  void SetUp() override {
    upTime = 1100;
    downTime = 1200;
    memset(&supla_esp_cfg, 0, sizeof(supla_esp_cfg));
    memset(&supla_esp_state, 0, sizeof(SuplaEspState));
    cleanupTimers();
    supla_esp_gpio_init_time = 0;
    gpioInitCb = *gpioCallbackFourRs;
  }

  void TearDown() override {
    upTime = 1100;
    downTime = 1200;
    memset(&supla_esp_cfg, 0, sizeof(supla_esp_cfg));
    memset(&supla_esp_state, 0, sizeof(SuplaEspState));
    supla_esp_gpio_init_time = 0;
    cleanupTimers();

    gpioInitCb = nullptr;
  }
};

TEST_F(RollerShutterAutoCalF, RsNotCalibrated_ServerReqRelayDown) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 1; //RS_RELAY_DOWN

  // Move RS down.
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);
  // simulate request from server
  supla_esp_channel_set_value(&reqValue);
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));

  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_GT(rsCfg->down_time, 7000);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

}

TEST_F(RollerShutterAutoCalF, RsNotCalibrated_DisableAutoCal) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (100) | (0 << 16);
  reqValue.value[0] = 1; //RS_RELAY_DOWN

  // Move RS down.
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);
  // simulate request from server
  supla_esp_channel_set_value(&reqValue);
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 10000);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 10000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));
}

TEST_F(RollerShutterAutoCalF, RsManuallyCalibrated_EnableAutoCal) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_cfg.Time1[0] = 1000;
  supla_esp_cfg.Time2[0] = 1000;
  supla_esp_state.rs_position[0] = 200;
  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 200);
  EXPECT_EQ(*rsCfg->full_opening_time, 1000);
  EXPECT_EQ(*rsCfg->full_closing_time, 1000);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 1000);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 20; // position 10

  // Move RS down.
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);
  // simulate request from server
  supla_esp_channel_set_value(&reqValue);
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  // +1500 ms
  for (int i = 0; i < 50; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_GT(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
}

TEST_F(RollerShutterAutoCalF,
    RsManuallyCalibrated_ApplyAnotherManualCalibration) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_cfg.Time1[0] = 1000;
  supla_esp_cfg.Time2[0] = 1000;
  supla_esp_state.rs_position[0] = 200;
  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 200);
  EXPECT_EQ(*rsCfg->full_opening_time, 1000);
  EXPECT_EQ(*rsCfg->full_closing_time, 1000);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 1000);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (10) | (0 << 16);
  reqValue.value[0] = 1; //RS_RELAY_DOWN

  // Move RS down.
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);
  // simulate request from server
  supla_esp_channel_set_value(&reqValue);
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 1000);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));
}

TEST_F(RollerShutterAutoCalF, RsAutoCalibrated_DisableAutoCal) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_cfg.Time1[0] = 0;
  supla_esp_cfg.Time2[0] = 0;
  supla_esp_cfg.AutoCalCloseTime[0] = 1000;
  supla_esp_cfg.AutoCalOpenTime[0] = 1000;
  supla_esp_state.rs_position[0] = 200;
  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 200);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1000);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (10 << 16);
  reqValue.value[0] = 1; //RS_RELAY_DOWN

  // Move RS down.
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);
  // simulate request from server
  supla_esp_channel_set_value(&reqValue);
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 1000);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 1000);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));
}

TEST_F(RollerShutterAutoCalF, RsAutoCalibrated_SetNewPosition) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_cfg.Time1[0] = 0;
  supla_esp_cfg.Time2[0] = 0;
  supla_esp_cfg.AutoCalCloseTime[0] = 1000;
  supla_esp_cfg.AutoCalOpenTime[0] = 1000;
  supla_esp_state.rs_position[0] = 200;
  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 200);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1000);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 30; // target posiotion = 20

  // Move RS down.
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);
  // simulate request from server
  supla_esp_channel_set_value(&reqValue);
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 200); // postion 10
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1000);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
}

TEST_F(RollerShutterAutoCalF, RsNotCalibrated_TriggerAutoCalServerStop) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 0; //RS_RELAY_OFF

  // STOP RS.
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);
  // simulate request from server
  supla_esp_channel_set_value(&reqValue);
  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration should not happen on STOP request
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

}

// Button actions should not start calibration
TEST_F(RollerShutterAutoCalF, RsNotCalibrated_ButtonMovementsThenServerReq) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_DOWN, 1, 0);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));

  // Move down for 15 s
  for (int i = 0; i < 1500; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // 
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 15000);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // check if buttons are still working 
  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_UP, 1, 0);
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }
  
  EXPECT_EQ(rsCfg->up_time, 990);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);

  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_OFF, 1, 0);

  for (int i = 0; i < 5; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 80+10; // position 80

  supla_esp_channel_set_value(&reqValue);

  // request from server should trigger autocal
  for (int i = 0; i < 900; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (80 * 100), 20);
  EXPECT_EQ(supla_esp_cfg.Time1[0], 0);
  EXPECT_EQ(supla_esp_cfg.Time2[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

}

TEST_F(RollerShutterAutoCalF, RsNotCalibrated_ButtonUp) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_UP, 1, 0);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // 
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_GT(rsCfg->up_time, 7000);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF, RsNotCalibrated_TriggerAutoCalAddTask0) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_add_task(0, 0);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration procedure should happen here
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 100);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF, RsNotCalibrated_TriggerAutoCalAddTask60) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_add_task(0, 60);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration procedure should happen here
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (60*100), 100);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF,
    RsNotCalibrated_TriggerAutoCalAndChangeTargetPosition) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_add_task(0, 60);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration procedure should start but it is not completed yet
  for (int i = 0; i < 100; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration is in progress
  EXPECT_TRUE(rsCfg->up_time > 0 || rsCfg->down_time > 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO) ||
      eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_GT(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_add_task(0, 20);

  // Calibration procedure should happen here
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (20*100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF,
    RsNotCalibrated_TriggerAutoCalAndAbortByButton) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_add_task(0, 60);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration procedure should start but it is not completed yet
  for (int i = 0; i < 250; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration is in progress
  EXPECT_TRUE(rsCfg->up_time > 0 || rsCfg->down_time > 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO) ||
      eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_GT(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_UP, 1, 0);

  // Calibration should be aborted, standart move up should happen
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_GT(rsCfg->up_time, 6000);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF,
    RsNotCalibrated_TriggerAutoCalAndChangeTargetPositionMix) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_add_task(0, 60);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration procedure should start but it is not completed yet
  for (int i = 0; i < 250; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration is in progress
  EXPECT_TRUE(rsCfg->up_time > 0 || rsCfg->down_time > 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO) ||
      eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_GT(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_add_task(0, 20);
  for (int i = 0; i < 50; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  supla_esp_gpio_rs_add_task(0, 90);
  for (int i = 0; i < 50; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 80+10; // position 80
  supla_esp_channel_set_value(&reqValue);

  // Calibration procedure should happen here
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (80*100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF,
    RsNotCalibrated_TriggerAutoCalAndAbort) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_add_task(0, 60);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration procedure should start but it is not completed yet
  for (int i = 0; i < 250; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration is in progress
  EXPECT_TRUE(rsCfg->up_time > 0 || rsCfg->down_time > 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO) ||
      eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_GT(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_OFF, 1, 0);

  // Calibration procedure should happen here
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF,
    RsNotCalibrated_TriggerAutoCalAndAbortFromServer) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  supla_esp_gpio_rs_add_task(0, 60);

  EXPECT_EQ(rsCfg->delayed_trigger.value, 0);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // Calibration procedure should start but it is not completed yet
  for (int i = 0; i < 250; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration is in progress
  EXPECT_TRUE(rsCfg->up_time > 0 || rsCfg->down_time > 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO) ||
      eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_GT(rsCfg->autoCal_step, 0);

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 0; // STOP
  supla_esp_channel_set_value(&reqValue);

  // Calibration procedure should happen here
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF,
    RsNotCalibrated_TriggerAutoCalFromServerAbortByButtonAndButtonDown) {
  // add task may be executed i.e. in mqtt
  
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 10+10; // position 10
  supla_esp_channel_set_value(&reqValue);


  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);

  // Calibration procedure should start but it is not completed yet
  for (int i = 0; i < 250; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration is in progress
  EXPECT_TRUE(rsCfg->up_time > 0 || rsCfg->down_time > 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_TRUE(eagleStub.getGpioValue(UP_GPIO) ||
      eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_GT(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_OFF, 1, 0);

  // Calibration procedure shouldn't happen here
  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // Check if calibration was done correctly
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // down button should not trigger autocal
  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_DOWN, 1, 0);

  for (int i = 0; i < 800; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_GT(rsCfg->down_time, 6000);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterFourAutoCalF, AutoCalFourRS) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[1], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[1], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[2], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[2], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[3], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[3], 0);

  supla_roller_shutter_cfg_t *rsCfg1 = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg1, nullptr);

  supla_roller_shutter_cfg_t *rsCfg2 = supla_esp_gpio_get_rs__cfg(3);
  ASSERT_NE(rsCfg2, nullptr);

  supla_roller_shutter_cfg_t *rsCfg3 = supla_esp_gpio_get_rs__cfg(5);
  ASSERT_NE(rsCfg3, nullptr);

  supla_roller_shutter_cfg_t *rsCfg4 = supla_esp_gpio_get_rs__cfg(7);
  ASSERT_NE(rsCfg4, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  // RS 1
  EXPECT_EQ(rsCfg1->up_time, 0);
  EXPECT_EQ(rsCfg1->down_time, 0);
  EXPECT_EQ(*rsCfg1->position, 0);
  EXPECT_EQ(*rsCfg1->full_opening_time, 0);
  EXPECT_EQ(*rsCfg1->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(1));
  EXPECT_FALSE(eagleStub.getGpioValue(2));

  // RS 2
  EXPECT_EQ(rsCfg2->up_time, 0);
  EXPECT_EQ(rsCfg2->down_time, 0);
  EXPECT_EQ(*rsCfg2->position, 0);
  EXPECT_EQ(*rsCfg2->full_opening_time, 0);
  EXPECT_EQ(*rsCfg2->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(3));
  EXPECT_FALSE(eagleStub.getGpioValue(4));

  // RS 3
  EXPECT_EQ(rsCfg3->up_time, 0);
  EXPECT_EQ(rsCfg3->down_time, 0);
  EXPECT_EQ(*rsCfg3->position, 0);
  EXPECT_EQ(*rsCfg3->full_opening_time, 0);
  EXPECT_EQ(*rsCfg3->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(5));
  EXPECT_FALSE(eagleStub.getGpioValue(6));

  // RS 4
  EXPECT_EQ(rsCfg4->up_time, 0);
  EXPECT_EQ(rsCfg4->down_time, 0);
  EXPECT_EQ(*rsCfg4->position, 0);
  EXPECT_EQ(*rsCfg4->full_opening_time, 0);
  EXPECT_EQ(*rsCfg4->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(7));
  EXPECT_FALSE(eagleStub.getGpioValue(8));

  // Calibrate first RS
  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 10+10; // position 10
  supla_esp_channel_set_value(&reqValue);

  // Manual time calibration second RS
  reqValue.ChannelNumber = 1;
  reqValue.DurationMS = (9) | (8 << 16);
  reqValue.value[0] = 10+40; // position 40
  supla_esp_channel_set_value(&reqValue);

  // Change position without time on RS which doesn't support autocal, so
  // nothing should happen
  reqValue.ChannelNumber = 2;
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 10+40; // position 40
  supla_esp_channel_set_value(&reqValue);

  // Calibrate fourth RS
  reqValue.ChannelNumber = 3;
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 10+70; // position 70
  supla_esp_channel_set_value(&reqValue);

  // Move time by 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg1->up_time, 0);
  EXPECT_EQ(rsCfg1->down_time, 0);
  EXPECT_NEAR(*rsCfg1->position, 100 + (10 * 100), 90);
  EXPECT_EQ(*rsCfg1->full_opening_time, 0);
  EXPECT_EQ(*rsCfg1->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(1));
  EXPECT_FALSE(eagleStub.getGpioValue(2));
  EXPECT_EQ(rsCfg1->autoCal_step, 0);

  EXPECT_EQ(rsCfg2->up_time, 0);
  EXPECT_EQ(rsCfg2->down_time, 0);
  EXPECT_NEAR(*rsCfg2->position, 100 + (40 * 100), 90);
  EXPECT_EQ(*rsCfg2->full_opening_time, 800);
  EXPECT_EQ(*rsCfg2->full_closing_time, 900);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[1], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[1], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(3));
  EXPECT_FALSE(eagleStub.getGpioValue(4));
  EXPECT_EQ(rsCfg2->autoCal_step, 0);

  EXPECT_EQ(rsCfg3->up_time, 0);
  EXPECT_EQ(rsCfg3->down_time, 0);
  EXPECT_EQ(*rsCfg3->position, 0);
  EXPECT_EQ(*rsCfg3->full_opening_time, 0);
  EXPECT_EQ(*rsCfg3->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[2], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[2], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(5));
  EXPECT_FALSE(eagleStub.getGpioValue(6));
  EXPECT_EQ(rsCfg3->autoCal_step, 0);

  EXPECT_EQ(rsCfg4->up_time, 0);
  EXPECT_EQ(rsCfg4->down_time, 0);
  EXPECT_NEAR(*rsCfg4->position, 100 + (70 * 100), 90);
  EXPECT_EQ(*rsCfg4->full_opening_time, 0);
  EXPECT_EQ(*rsCfg4->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[3], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[3], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(7));
  EXPECT_FALSE(eagleStub.getGpioValue(8));
  EXPECT_EQ(rsCfg4->autoCal_step, 0);

}

TEST_F(RollerShutterAutoCalF, AutoCalibrationFromCfgmodeOrMqtt) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // manual calibration times
  supla_esp_gpio_rs_apply_new_times(0, 2000, 2200);
  supla_esp_gpio_rs_add_task(0, 50);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 2200);
  EXPECT_EQ(*rsCfg->full_closing_time, 2000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (50 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 2200);
  EXPECT_EQ(*rsCfg->full_closing_time, 2000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // enable auto calibration 
  supla_esp_gpio_rs_apply_new_times(0, 0, 0);
  supla_esp_gpio_rs_add_task(0, 20);

  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (20 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // new times should be ignored
  supla_esp_gpio_rs_apply_new_times(0, 0, 0);
  supla_esp_gpio_rs_add_task(0, 30);

  // + 10 s
  for (int i = 0; i < 150; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (30 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // set new time manual calibration
  supla_esp_cfg.Time2[0] = 1000;
  supla_esp_cfg.Time1[0] = 1000;

  // + 1.5 s
  for (int i = 0; i < 150; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 1000);
  EXPECT_EQ(*rsCfg->full_closing_time, 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // trigger calibration with manual time
  supla_esp_gpio_rs_add_task(0, 33);
  
  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (33 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 1000);
  EXPECT_EQ(*rsCfg->full_closing_time, 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // enable autocalibration
  supla_esp_cfg.Time2[0] = 0;
  supla_esp_cfg.Time1[0] = 0;

  // + 1.5 s
  for (int i = 0; i < 150; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // trigger calibration with manual time
  supla_esp_gpio_rs_add_task(0, 53);
  
  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (53 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);


}

TEST_F(RollerShutterAutoCalF, SwitchToManualCalibrationDuringAutoCalibration) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // manual calibration times
  supla_esp_gpio_rs_apply_new_times(0, 0, 0);
  supla_esp_gpio_rs_add_task(0, 50);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // + 2 s
  for (int i = 0; i < 100; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_GT(rsCfg->autoCal_step, 0);

  // set new time manual calibration
  supla_esp_cfg.Time2[0] = 1000;
  supla_esp_cfg.Time1[0] = 1000;

  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (50*100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 1000);
  EXPECT_EQ(*rsCfg->full_closing_time, 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // trigger calibration with manual time
  supla_esp_gpio_rs_add_task(0, 33);
  
  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (33 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 1000);
  EXPECT_EQ(*rsCfg->full_closing_time, 1000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

}

TEST_F(RollerShutterAutoCalF, SwitchToAutoCalibrationDuringManualCalibration) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // manual calibration times
  supla_esp_gpio_rs_apply_new_times(0, 2000, 2000);
  supla_esp_gpio_rs_add_task(0, 20);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 2000);
  EXPECT_EQ(*rsCfg->full_closing_time, 2000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // + 2 s
  for (int i = 0; i < 100; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_GT(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 2000);
  EXPECT_EQ(*rsCfg->full_closing_time, 2000);
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  // enable auto cal
  supla_esp_cfg.Time2[0] = 0;
  supla_esp_cfg.Time1[0] = 0;

  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (20*100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_add_task(0, 33);
  
  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (33 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

}

TEST_F(RollerShutterAutoCalF, SwitchToAutoCalibrationDuringMoveDown) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // manual calibration times
  supla_esp_gpio_rs_apply_new_times(0, 2000, 2000);
  supla_esp_gpio_rs_add_task(0, 20);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 2000);
  EXPECT_EQ(*rsCfg->full_closing_time, 2000);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100+(20*100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 2000);
  EXPECT_EQ(*rsCfg->full_closing_time, 2000);
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_set_relay(rsCfg, RS_RELAY_DOWN, 1, 0);
  // + 0.5 s
  for (int i = 0; i < 50; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));

  // enable auto cal
  supla_esp_cfg.Time2[0] = 0;
  supla_esp_cfg.Time1[0] = 0;

  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_GT(rsCfg->down_time, 6000);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_TRUE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

  supla_esp_gpio_rs_add_task(0, 33);
  
  // + 10 s
  for (int i = 0; i < 1000; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_NEAR(*rsCfg->position, 100 + (33 * 100), 90);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 1100);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 1200);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

}

TEST_F(RollerShutterAutoCalF, AutoCalibrationFailedTooShortTime) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  // Set how long [ms] "is_in_move" method will return true
  // auto cal times < 500 ms are considered as errors
  upTime = 400;
  downTime = 400;

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 30; // position 20

  supla_esp_channel_set_value(&reqValue);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);

  // Calibration 
  for (int i = 0; i < 400; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);

}

TEST_F(RollerShutterAutoCalF, AutoCalibrationFailedTooShortTimeOnMoveUp) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  // Set how long [ms] "is_in_move" method will return true
  // auto cal times < 500 ms are considered as errors
  upTime = 400;
  downTime = 800;

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 30; // position 20

  supla_esp_channel_set_value(&reqValue);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);

  // Calibration 
  for (int i = 0; i < 400; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF, AutoCalibrationFailedTooLongTime) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  // Set how long [ms] "is_in_move" method will return true
  // auto cal times > 600000 (10 min) are considered as errors
  upTime = 700000;
  downTime = 700000;

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 30; // position 20

  supla_esp_channel_set_value(&reqValue);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);

  // Calibration 
  for (int i = 0; i < 7005; i++) {
    curTime += 100000; // +100ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}

TEST_F(RollerShutterAutoCalF, AutoCalibrationFailedTooLongDownTime) {
  int curTime = 10000; // start at +10 ms
  EXPECT_CALL(time, system_get_time()).WillRepeatedly(ReturnPointee(&curTime));

  // Set how long [ms] "is_in_move" method will return true
  // auto cal times > 600000 (10 min) are considered as errors
  upTime = 7000;
  downTime = 700000;

  supla_esp_gpio_init();

  supla_roller_shutter_cfg_t *rsCfg = supla_esp_gpio_get_rs__cfg(1);
  ASSERT_NE(rsCfg, nullptr);

  os_timer_func_t *rsTimerCb = lastTimerCb;
  ASSERT_NE(rsTimerCb, nullptr);

  // +2000 ms
  for (int i = 0; i < 200; i++) {
    curTime += 10000; // +10ms
    executeTimers();
  }

  // nothing should change
  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));

  TSD_SuplaChannelNewValue reqValue = {};
  reqValue.ChannelNumber = 0;
  // closing and opening time is coded in 0.1s units on 2x16 bit blocks of
  // DurationMS
  // In autocalibration, server sends ct/ot == 0
  reqValue.DurationMS = (0) | (0 << 16);
  reqValue.value[0] = 30; // position 20

  supla_esp_channel_set_value(&reqValue);

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);

  // Calibration 
  for (int i = 0; i < 7005; i++) {
    curTime += 100000; // +100ms
    executeTimers();
  }

  EXPECT_EQ(rsCfg->up_time, 0);
  EXPECT_EQ(rsCfg->down_time, 0);
  EXPECT_EQ(*rsCfg->position, 0);
  EXPECT_EQ(*rsCfg->full_opening_time, 0);
  EXPECT_EQ(*rsCfg->full_closing_time, 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalOpenTime[0], 0);
  EXPECT_EQ(supla_esp_cfg.AutoCalCloseTime[0], 0);
  EXPECT_FALSE(eagleStub.getGpioValue(UP_GPIO));
  EXPECT_FALSE(eagleStub.getGpioValue(DOWN_GPIO));
  EXPECT_EQ(rsCfg->autoCal_step, 0);
}
/* TODO:
 * - calibration on calcfg request - feedback in progress?
 */