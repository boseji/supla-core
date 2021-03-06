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

#include "mqtt_unpublisher_user_topic_provider.h"
#include <stdlib.h>
#include <string.h>

supla_mqtt_unpublisher_user_topic_provider::
    supla_mqtt_unpublisher_user_topic_provider(void)
    : supla_mqtt_message_provider() {
  row = NULL;
  unsubscribe = NULL;
}

supla_mqtt_unpublisher_user_topic_provider::
    ~supla_mqtt_unpublisher_user_topic_provider(void) {}

bool supla_mqtt_unpublisher_user_topic_provider::get_message_at_index(
    unsigned short index, const char *topic_prefix, char **topic_name,
    void **message, size_t *message_size) {
  if (row != NULL) {
    switch (index) {
      case 0:

        return create_message(topic_prefix, row->user_suid, topic_name, NULL,
                              NULL, NULL, false, "#");
        break;
      case 1:
        if (topic_name) {
          const char format[] = "homeassistant/+/%s/+/config";
          size_t len = snprintf(NULL, 0, format, row->user_suid);
          if (len) {
            len++;
            *topic_name = (char *)malloc(len);
            snprintf(*topic_name, len, format, row->user_suid);
          }

          return *topic_name != NULL;
        }
    }
  }

  return false;
}

bool supla_mqtt_unpublisher_user_topic_provider::fetch(const char *topic_prefix,
                                                       char **topic_name,
                                                       bool *unsubscribe) {
  if (supla_mqtt_message_provider::fetch(topic_prefix, topic_name, NULL,
                                         NULL)) {
    if (unsubscribe) {
      *unsubscribe = this->unsubscribe;
    }

    return true;
  }

  return false;
}

void supla_mqtt_unpublisher_user_topic_provider::set_data_row(
    _mqtt_db_data_row_user_t *row, bool unsubscribe) {
  this->row = row;
  this->unsubscribe = unsubscribe;
  reset_index();
}
