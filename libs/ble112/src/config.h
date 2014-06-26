/*
 * BLELabs BLE112-Protostick Demo Application for Tutorial #3
 * Contact: support@blelabs.com
 * Author: PeterVanPansen
 * Date: 12.08.2013
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 BLELabs
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "./apitypes.h"

// https://developer.bluetooth.org/gatt/declarations/Pages/DeclarationsHome.aspx
#define GATT_PRIMARY_SERVICE_UUID {0x00, 0x28}
#define GATT_SECONDARY_SERVICE_UUID {0x01, 0x28}
#define GATT_INLCUDE_DECLARATION_UUID {0x02, 0x28}
// BLUETOOTH SPECIFICATION Version 4.0 [Vol 3] page 537 of 656
#define GATT_CHARACTERISTIC_DECLARATION_UUID {0x03, 0x28}
// https://developer.bluetooth.org/gatt/descriptors/Pages/DescriptorsHomePage.aspx
#define GATT_CHARACTERISTIC_EXTENTED_PROPERTIES_UUID {0x00, 0x29}
#define GATT_CHARACTERISTIC_USER_DESCRIPTION_UUID {0x01, 0x29}
#define GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID {0x02, 0x29}
// https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicsHome.aspx
#define GATT_DEVICENAME_UUID {0x00, 0x2A}
#define GATT_APPEARANCE_UUID {0x01, 0x2A}

// Attribute Value Types
#define ATTCLIENT_ATTRIBUTE_VALUE_TYPE_READ 0
#define ATTCLIENT_ATTRIBUTE_VALUE_TYPE_NOTIFY 1
#define ATTCLIENT_ATTRIBUTE_VALUE_TYPE_INDICATE 2
#define ATTCLIENT_ATTRIBUTE_VALUE_TYPE_READ_BY_TYPE 3
#define ATTCLIENT_ATTRIBUTE_VALUE_TYPE_READ_BLOB 4
#define ATTCLIENT_ATTRIBUTE_VALUE_TYPE_INDICATE_RSP_REQ 5

/*
 * These following variables are for global communication between
 * API Layers.
 * */
#define APP_DEVICE_INIT 0
#define APP_DEVICE_FOUND 1
#define APP_DEVICE_CONNECTED 2
#define APP_DEVICE_DICONNECTED 3

#define APP_ATTCLIENT_ERROR (1<<0)
#define APP_ATTCLIENT_NOTIFICATION_PENDING (1<<1)
#define APP_ATTCLIENT_PENDING (1<<2)
#define APP_ATTCLIENT_VALUE_PENDING (1<<3)

#define APP_COMMAND_ERROR (1<<3)
#define APP_COMMAND_PENDING (1<<4)

#define APP_OK    0
#define APP_FAILURE -1

/* Little helpers */
#define setFlag(target, flag) (target |= flag)
#define clearFlag(target, flag) (target &= ~(flag))
#define issetFlag(target, flag) ((target & flag)? 1:0)
#define clearFlags(target) (target = 0)

/* BLE connection settings */
typedef struct hci_connection {
    bd_addr target;
    uint8 addr_type;
    uint16 conn_interval_min;
    uint16 conn_interval_max;
    uint16 timeout;
    uint16 latency;
    uint16 state;
    uint8 handle;
} hci_connection_t;
extern hci_connection_t app_connection;


/* BLE attclient data */
typedef struct hci_attclient {
    uint16 state;
    uint16 handle;
    struct {
        uint8 len;
        uint8 *data;
    } value;
} hci_attclient_t;
extern hci_attclient_t app_attclient;


/* State flag */
extern uint16 app_state;


#endif  // INC_CONFIG_H_
