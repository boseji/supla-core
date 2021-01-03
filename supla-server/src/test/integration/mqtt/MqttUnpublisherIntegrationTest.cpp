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

#include "MqttUnpublisherIntegrationTest.h"
#include "log.h"
#include "mqtt_unpublisher.h"
#include "mqtt_unpublisher_datasource.h"

namespace testing {

MqttUnpublisherIntegrationTest::MqttUnpublisherIntegrationTest()
    : MqttClientIntegrationTest() {}

MqttUnpublisherIntegrationTest::~MqttUnpublisherIntegrationTest() {}

supla_mqtt_client *MqttUnpublisherIntegrationTest::clientInit(
    supla_mqtt_client_library_adapter *library_adapter,
    supla_mqtt_client_settings *settings,
    supla_mqtt_client_datasource *datasource) {
  return new supla_mqtt_unpublisher(library_adapter, settings, datasource);
}

supla_mqtt_client_datasource *MqttUnpublisherIntegrationTest::dsInit(
    supla_mqtt_client_settings *settings) {
  return new supla_mqtt_unpublisher_datasource(settings);
}

supla_mqtt_unpublisher_datasource *MqttUnpublisherIntegrationTest::getDS(void) {
  return static_cast<supla_mqtt_unpublisher_datasource *>(
      MqttClientIntegrationTest::getDS());
}

void MqttUnpublisherIntegrationTest::SetUp() {
  initTestDatabase();
  runSqlScript("DataForMqttTests.sql");
  runSqlScript("DisableMqttForUser2645.sql");

  MqttClientIntegrationTest::SetUp();
}

TEST_F(MqttUnpublisherIntegrationTest, disableAlreadyDisabled) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  getDS()->on_userdata_changed(2645);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);
}

TEST_F(MqttUnpublisherIntegrationTest, enableDisabledAndDisableAgain) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  runSqlScript("EnableMqttForUser2645.sql");

  getDS()->on_userdata_changed(2645);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  runSqlScript("DisableMqttForUser2645.sql");

  getDS()->on_userdata_changed(2645);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 2);

  ASSERT_EQ(
      getLibAdapter()->subscribed_pop().compare("supla/2645test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->subscribed_pop().compare(
                "homeassistant/+/2645test_supla_org/+/config"),
            0);

  sleep(22);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 2);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  ASSERT_EQ(
      getLibAdapter()->unsubscribed_pop().compare("supla/2645test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->unsubscribed_pop().compare(
                "homeassistant/+/2645test_supla_org/+/config"),
            0);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  runSqlScript("EnableMqttForUser2645.sql");

  getDS()->on_userdata_changed(2645);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  runSqlScript("DisableMqttForUser2645.sql");

  getDS()->on_userdata_changed(2645);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 2);

  ASSERT_EQ(
      getLibAdapter()->subscribed_pop().compare("supla/2645test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->subscribed_pop().compare(
                "homeassistant/+/2645test_supla_org/+/config"),
            0);

  runSqlScript("EnableMqttForUser2645.sql");

  getDS()->on_userdata_changed(2645);
  sleep(2);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 2);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  ASSERT_EQ(
      getLibAdapter()->unsubscribed_pop().compare("supla/2645test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->unsubscribed_pop().compare(
                "homeassistant/+/2645test_supla_org/+/config"),
            0);
}

TEST_F(MqttUnpublisherIntegrationTest, disableEnabled) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  runSqlScript("DisableMqttForUser48.sql");

  getDS()->on_userdata_changed(48);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 2);

  ASSERT_EQ(
      getLibAdapter()->subscribed_pop().compare("supla/48test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->subscribed_pop().compare(
                "homeassistant/+/48test_supla_org/+/config"),
            0);

  getLibAdapter()->on_message_received("supla/48test@supla.org/1", "123");
  getLibAdapter()->on_message_received("supla/48test@supla.org/2", "123");
  getLibAdapter()->on_message_received("supla/48test@supla.org/3", "123");
  getLibAdapter()->on_message_received("supla/48test@supla.org/4", "123");
  getLibAdapter()->on_message_received("supla/48test@supla.org/5", "123");

  sleep(22);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 2);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  // print_expected();

  const char *expectedData[] = {
      "supla/48test@supla.org/1", NULL, "supla/48test@supla.org/2", NULL,
      "supla/48test@supla.org/3", NULL, "supla/48test@supla.org/4", NULL,
      "supla/48test@supla.org/5", NULL};

  verify_published(expectedData, sizeof(expectedData) / sizeof(void *));

  ASSERT_EQ(
      getLibAdapter()->unsubscribed_pop().compare("supla/48test@supla.org/#"),
      0);
  ASSERT_EQ(getLibAdapter()->unsubscribed_pop().compare(
                "homeassistant/+/48test_supla_org/+/config"),
            0);
}

TEST_F(MqttUnpublisherIntegrationTest, enableDuringDisabling) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  runSqlScript("DisableMqttForUser48.sql");

  getDS()->on_userdata_changed(48);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 2);

  ASSERT_EQ(
      getLibAdapter()->subscribed_pop().compare("supla/48test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->subscribed_pop().compare(
                "homeassistant/+/48test_supla_org/+/config"),
            0);

  sleep(5);

  runSqlScript("EnableMqttForUser48.sql");

  getDS()->on_userdata_changed(48);
  sleep(2);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 2);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  ASSERT_EQ(
      getLibAdapter()->unsubscribed_pop().compare("supla/48test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->unsubscribed_pop().compare(
                "homeassistant/+/48test_supla_org/+/config"),
            0);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  runSqlScript("DisableMqttForUser48.sql");

  getDS()->on_userdata_changed(48);
  sleep(2);

  ASSERT_FALSE(getDS()->is_context_open());
  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 2);

  ASSERT_EQ(
      getLibAdapter()->subscribed_pop().compare("supla/48test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->subscribed_pop().compare(
                "homeassistant/+/48test_supla_org/+/config"),
            0);

  sleep(22);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 2);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  ASSERT_EQ(
      getLibAdapter()->unsubscribed_pop().compare("supla/48test@supla.org/#"),

      0);

  ASSERT_EQ(getLibAdapter()->unsubscribed_pop().compare(
                "homeassistant/+/48test_supla_org/+/config"),
            0);
}

TEST_F(MqttUnpublisherIntegrationTest, deleteDeviceWithTimeout) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  getDS()->before_device_delete(743, 506);
  sleep(15);
  getDS()->on_device_deleted(743, 506);
  sleep(2);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);
}

TEST_F(MqttUnpublisherIntegrationTest, onDeviceDeleted) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  getDS()->before_device_delete(117, 36);
  getDS()->before_device_delete(743, 506);
  sleep(2);
  getDS()->on_device_deleted(117, 36);
  getDS()->on_device_deleted(743, 506);
  waitForPublications(120);

  // print_expected();

  const char *expectedData[] = {
      "supla/117test@supla.org/devices/36/enabled",
      NULL,
      "supla/117test@supla.org/devices/36/last_connected",
      NULL,
      "supla/117test@supla.org/devices/36/last_ipv4",
      NULL,
      "supla/117test@supla.org/devices/36/manufacturer",
      NULL,
      "supla/117test@supla.org/devices/36/name",
      NULL,
      "supla/117test@supla.org/devices/36/proto_ver",
      NULL,
      "supla/117test@supla.org/devices/36/soft_ver",
      NULL,
      "supla/117test@supla.org/devices/36/channels/50/type",
      NULL,
      "supla/117test@supla.org/devices/36/channels/50/function",
      NULL,
      "supla/117test@supla.org/devices/36/channels/50/caption",
      NULL,
      "supla/117test@supla.org/devices/36/channels/50/hidden",
      NULL,
      "homeassistant/light/117test_supla_org/50/config",
      NULL,
      "supla/117test@supla.org/devices/36/channels/50/state/connected",
      NULL,
      "supla/117test@supla.org/devices/36/channels/50/state/on",
      NULL,
      "supla/117test@supla.org/devices/36/channels/51/type",
      NULL,
      "supla/117test@supla.org/devices/36/channels/51/function",
      NULL,
      "supla/117test@supla.org/devices/36/channels/51/caption",
      NULL,
      "supla/117test@supla.org/devices/36/channels/51/hidden",
      NULL,
      "homeassistant/light/117test_supla_org/51/config",
      NULL,
      "supla/117test@supla.org/devices/36/channels/51/state/connected",
      NULL,
      "supla/117test@supla.org/devices/36/channels/51/state/on",
      NULL,
      "supla/743test@supla.org/devices/506/enabled",
      NULL,
      "supla/743test@supla.org/devices/506/last_connected",
      NULL,
      "supla/743test@supla.org/devices/506/last_ipv4",
      NULL,
      "supla/743test@supla.org/devices/506/manufacturer",
      NULL,
      "supla/743test@supla.org/devices/506/name",
      NULL,
      "supla/743test@supla.org/devices/506/proto_ver",
      NULL,
      "supla/743test@supla.org/devices/506/soft_ver",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/type",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/function",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/caption",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/hidden",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_0/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_1/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_2/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_3/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_4/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_5/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_6/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_7/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_8/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_9/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_10/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_11/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_12/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_13/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_14/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_15/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_16/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_17/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_18/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_19/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_20/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_21/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_22/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_23/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_24/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_25/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_26/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_27/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_28/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_29/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_30/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_31/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_32/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_33/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_34/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_35/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_36/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_37/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_38/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_39/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_40/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_41/config",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/connected",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/total_cost",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_cost_balanced",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/price_per_unit",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/currency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/support",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_forward_active_energy_balanced",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_reverse_active_energy_balanced",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_forward_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_reverse_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "frequency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/voltage",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/current",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_active",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_reactive",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_apparent",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_factor",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "phase_angle",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_forward_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_reverse_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "frequency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/voltage",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/current",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_active",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_reactive",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_apparent",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_factor",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "phase_angle",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_forward_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_reverse_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "frequency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/voltage",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/current",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_active",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_reactive",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_apparent",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_factor",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "phase_angle",
      NULL};

  verify_published(expectedData, sizeof(expectedData) / sizeof(void *));

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);
}

TEST_F(MqttUnpublisherIntegrationTest, changeChannelFunctionWithTimeout) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  getDS()->before_channel_function_change(743, 966);
  runSqlScript("SetFunctionToNoneForChannel966.sql");
  sleep(25);
  getDS()->on_devicedata_changed(743, 506);
  sleep(2);

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);
}

TEST_F(MqttUnpublisherIntegrationTest, changeChannelFunction) {
  waitForConnection();

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->published_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);

  getDS()->before_channel_function_change(743, 966);
  runSqlScript("SetFunctionToNoneForChannel966.sql");
  sleep(5);
  getDS()->on_devicedata_changed(743, 506);

  waitForPublications(92);

  // print_expected();

  const char *expectedData[] = {
      "supla/743test@supla.org/devices/506/channels/966/type",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/function",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/caption",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/hidden",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_0/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_1/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_2/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_3/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_4/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_5/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_6/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_7/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_8/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_9/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_10/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_11/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_12/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_13/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_14/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_15/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_16/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_17/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_18/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_19/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_20/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_21/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_22/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_23/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_24/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_25/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_26/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_27/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_28/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_29/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_30/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_31/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_32/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_33/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_34/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_35/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_36/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_37/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_38/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_39/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_40/config",
      NULL,
      "homeassistant/sensor/743test_supla_org/966_41/config",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/connected",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/total_cost",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_cost_balanced",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/price_per_unit",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/currency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/support",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_forward_active_energy_balanced",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/"
      "total_reverse_active_energy_balanced",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_forward_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "total_reverse_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "frequency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/voltage",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/current",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_active",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_reactive",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_apparent",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "power_factor",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/1/"
      "phase_angle",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_forward_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "total_reverse_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "frequency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/voltage",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/current",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_active",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_reactive",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_apparent",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "power_factor",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/2/"
      "phase_angle",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_forward_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_reverse_active_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_forward_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "total_reverse_reactive_energy",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "frequency",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/voltage",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/current",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_active",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_reactive",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_apparent",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "power_factor",
      NULL,
      "supla/743test@supla.org/devices/506/channels/966/state/phases/3/"
      "phase_angle",
      NULL};

  verify_published(expectedData, sizeof(expectedData) / sizeof(void *));

  ASSERT_EQ(getLibAdapter()->unsubscribed_count(), 0);
  ASSERT_EQ(getLibAdapter()->subscribed_count(), 0);
}

} /* namespace testing */
