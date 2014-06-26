#ifndef INC_SIMPLESERIAL_H_
#define INC_SIMPLESERIAL_H_

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
 *
 * Resources that were helpful for creating this class:
 * - http://www.webalice.it/fede.tft/serial_port/serial_port.html
 * - http://www.college-code.com/blog/wp-content/uploads/2008/11/boost_serial_port_demo.cpp
 * - http://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/
 * - http://www.gamedev.net/blog/950/entry-2249317-a-guide-to-getting-started-with-boostasio/
 * - http://stackoverflow.com/questions/6672724/boost-asio-socket-read-n-bytes-not-more-not-less-and-wait-until-they-come-or-tim
 * - http://stackoverflow.com/questions/7783237/sending-raw-data-with-write-in-boostasio
 * - http://stackoverflow.com/questions/6866078/how-to-send-raw-binary-data-using-boostasio
 *
 */


#include <cstddef>
#include <boost/asio.hpp>
#include <string>

#include "./apitypes.h"
#include "./cmd_def.h"


// don't pollute namespace (no 'using namespace'),
// but make typing a little easier
namespace bio = ::boost::asio;
typedef ::bio::serial_port_base bio_spb;


class SimpleSerial {
    private:
    static ::bio::io_service*  io_srvc_;
    static ::bio::serial_port* srl_port_;

    public:
    static void InitSerial(std::string port, uint32 baud_rate, uint8 char_size,
                    bio_spb::parity::type parity_type,
                    bio_spb::stop_bits::type stop_bits_type,
                    bio_spb::flow_control::type flow_ctrl_type);

    static void WriteBleMessage(uint8 len1, uint8* data1,
                                   uint16 len2, uint8* data2);

    static int ReadBleHeader(ble_header& api_header);

    static int ReadBleMessage(ble_header& api_header);


};


#endif  // INC_SIMPLESERIAL_H_
