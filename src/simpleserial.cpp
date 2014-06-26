
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Simon Wiesmann
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
 *
 */


#include <string>

#include "simpleserial.h"
#include <cstddef>

::bio::io_service*  SimpleSerial::io_srvc_  = nullptr;
::bio::serial_port* SimpleSerial::srl_port_ = nullptr;




/**
 * @brief Initializes the serial port.
 *
 * @param port              RS-232 interface to use (e.g. "com2" in windows, "/dev/ttyUSB0" in Linux)
 * @param baud_rate         the interface's baud rate
 * @param char_size         size in bit of characters to be transmitted
 * @param parity_type       use parity bit? even/odd/none?
 * @param stop_bits_type    how many stop bits? one/onepointfive/two
 * @param flow_ctrl_type    use flow control? hardware(rts,cts)/software/none
 */
void SimpleSerial::InitSerial(std::string port, uint32 baud_rate,
                              uint8 char_size,
                              bio_spb::parity::type parity_type,
                              bio_spb::stop_bits::type stop_bits_type,
                              bio_spb::flow_control::type flow_ctrl_type) {
    // create the service and serial port objects
    io_srvc_  = new ::bio::io_service();
    srl_port_ = new ::bio::serial_port(*io_srvc_, port);

    // baud rate
    //srl_port_->set_option(bio_spb::baud_rate(baud_rate));
    srl_port_->set_option(bio_spb::baud_rate(baud_rate));

    // character size
    srl_port_->set_option(bio_spb::character_size(
                           bio_spb::character_size(char_size)));

    // parity bit
    srl_port_->set_option(bio_spb::parity(parity_type));

    // stop bits
    srl_port_->set_option(bio_spb::stop_bits(stop_bits_type));

    // flow control
    srl_port_->set_option(bio_spb::flow_control(flow_ctrl_type));
}


int SimpleSerial::ReadBleHeader(ble_header& api_header) {
    return read(*srl_port_, bio::buffer((unsigned char*) &api_header, 4));
}


/**
 * @brief   reads and processes incomming data on the serial port
 * @return  0 if successful, -1 in case of error
 */
int SimpleSerial::ReadBleMessage(ble_header& api_header) {
    uint32 bytes_read;
    const struct ble_msg *api_msg;
    // struct ble_header api_header;
    unsigned char data[256];    // enough for BLE


    // ---------- read header ----------
    // bytes_read = read(*srl_port_, bio::buffer((unsigned char*) &api_header, 4));

    // reset and re-use for reading the payload
    bytes_read = 0;


    // ---------- read payload ----------
    // read rest if needed
    if (api_header.lolen) {
        bytes_read = bio::read(*srl_port_, bio::buffer(data, api_header.lolen));
    }

    // printf("payload: %lu byte \n", bytes_read);

    api_msg = ble_get_msg_hdr(api_header);


    if (!api_msg) {
        printf("ERROR: Message not found:%d:%d\n",
               static_cast<int>(api_header.cls),
               static_cast<int>(api_header.command));
        return -1;
    }


    // run the handler for this message type.
    // the handler funcs are in command.c
    api_msg->handler(data);

    return 0;
}


/**
 * @brief SimpleSerial::WriteBleMessage
 * @param len1      no. of byte to transmit from variable data1
 * @param data1     first chunk of data to send
 * @param len2      no. of byte to transmit from variable data2
 * @param data2     second chunk of data to send
 */
void SimpleSerial::WriteBleMessage(uint8 len1, uint8* data1,
                                    uint16 len2, uint8* data2) {
    bio::write(*srl_port_, bio::buffer(reinterpret_cast<void*>(data1), len1));
    bio::write(*srl_port_, bio::buffer(reinterpret_cast<void*>(data2), len2));
}




