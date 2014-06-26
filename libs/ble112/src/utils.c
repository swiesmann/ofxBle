/*
 * BLELabs BLE112-Protostick Demo Application for Tutorial #3
 * Contact: support@blelabs.com
 * Author: PeterVanPansen
 * Date: 12.08.2013
 * License: See license file in package
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "apitypes.h"
#include "config.h"
#include "utils.h"

/*
 * Implementations
 * */

/* Lil'helper */
inline void printOffset(char sign, int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("%c", ' ');
    }
}

/* Format example (... -> Offset):
 * ... XX XX XX XX XX XX XX XX
 * ... XX XX XX XX XX XX XX XX
 * ... XX XX XX
 * */
void printHexdump(uint8 *data, int len, int offset) {
    printOffset(' ', offset);
    int i;
    for (i = 1; i <= len; i++) {
        printf("%02x%s", data[i - 1], i % 8 ? " " : "\n");
        if ((i % 8) == 0) {
            printOffset(' ', offset);
        }
    }
}

/* Format example: XX:XX:XX:XX:XX:XX */
void printAddr6(uint8 *addr, char token) {
    /* I am lazy and dont want to change the loop */
    uint8 buf[6];
    memcpy(buf, addr, 6);
    reverseArray(buf, 6);
    addr = buf;
    int i;
    for (i = 0; i < 6; i++) {
        printf("%02x", addr[i]);
        if (i + 1 != 6) {
            printf("%c", token);
        }
    }
}

/* Format 128-bit UUID: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX */
/* Format  16-bit UUID: XXXX */
void printUUID(uint8 *uuid, int len) {
    if (len > 16)
        return;

    /* I am lazy and dont want to change indices */
    uint8 buf[16];
    memcpy(buf, uuid, len);
    reverseArray(buf, len);
    uuid = buf;
    if (len == 16) {
        printf("%02x%02x%02x%02x-", uuid[0], uuid[1], uuid[2], uuid[3]);
        printf("%02x%02x-", uuid[4], uuid[5]);
        printf("%02x%02x-", uuid[6], uuid[7]);
        printf("%02x%02x-", uuid[8], uuid[9]);
        printf("%02x%02x%02x%02x%02x%02x", uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
    } else {
        int i;
        for (i = 0; i < len; i++) {
            printf("%02x", uuid[i]);
        }
    }
}

uint8 *reverseArray(uint8 *array, size_t len) {
    if (len <= 1)
        return array;

    int begin = 0;
    int end = len - 1;
    uint8 buf;
    while (1) {
        if (array[begin] != array[end]) {
            buf = array[end];
            array[end] = array[begin];
            array[begin] = buf;
        }
        if (++begin >= end--) {
            break;
        }
    }
    return array;
}

/* Allowed formats (hex with space and/or line token) i.e.:
 * XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
 * XXXXXXXX XXXX XXXX XXXX XXXXXXXXXXXX
 * XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 * X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
 *
 * !!! Lengths are not checked at all so watch your steps !!!
 *
 * */
void uuid128StrToArray(char str_uuid[], uint8 arr_uuid[]) {
    char *pch, *pbuf, cbuf[3] = {0};
    int i, counter, len;
    long int ibuf;
    counter = 0;
    pch = strtok(str_uuid, " -");
    while (pch != NULL ) {
        len = strlen(pch);
        pbuf = pch;
        for (i = 0; i < len;i+=2) {
            strncpy(cbuf, pbuf, 2);
            pbuf += 2;
            ibuf = strtol(cbuf, NULL, 16);
            arr_uuid[counter++] = (uint8) ibuf;
        }
        pch = strtok(NULL, " -");
    }
}
