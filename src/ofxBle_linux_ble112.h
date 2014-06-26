#ifndef _OFXBLE_LINUX_BLE112_H
#define _OFXBLE_LINUX_BLE112_H

#include "ofMain.h"


#include "simpleserial.h"
#include "ble112/src/utils.h"


class ofxBleThread : public ofThread {
    public:

        static ofEvent<uint16_t> onAcceleratorX;
        static ofEvent<uint16_t> onAcceleratorY;


        // ----- CONSTRUCTOR -----
        ofxBleThread() {

            serial_one = "/dev/ttyUSB0";

            printf("connecting via %s...\n", serial_one);

            SimpleSerial::InitSerial(serial_one,        // serial port
                115200,                                 // baud rate
                8,                                      // character size
                bio_spb::parity::none,                  // no parity bit
                bio_spb::stop_bits::one,                // one stop bit
                bio_spb::flow_control::hardware);       // hardware flow control

            // tell the bluegiga library which function to use for serial output
            bglib_output = SimpleSerial::WriteBleMessage;

            // Target MAC address
            // TODO(you): change this to the address of your target BLE board
            bd_addr target = { { 0x00, 0x07, 0x80, 0x6a, 0xf2, 0x89 } };

            // API needs address reversed
            reverseArray(target.addr, sizeof(target.addr));

            // BLE settings
            app_connection.addr_type = 0;
            app_connection.target = target;
            app_connection.conn_interval_min = 80;      /*100ms*/
            app_connection.conn_interval_max = 3200;    /*1s*/
            app_connection.timeout = 1000;              /*10s*/
            app_connection.latency = 0;                 /*0ms*/
            app_connection.addr_type = 0;

            // Provide place to store data
            app_attclient.value.data = value_buffer;
            app_attclient.value.len  = 0;

            // Initialize status flags
            app_connection.state = APP_DEVICE_INIT;
            app_attclient.state  = 0;
            app_state            = 0;

            /* BLE system reset */
            printf("[>] ble_cmd_system_reset\n");
            ble_cmd_system_reset(0); //Reboot normal program
            if (wait_for_rsp() != APP_OK) {
                die();
            }

            // But wait a second until device boots up
            printf("    sleep some (wait for device to boot)\n");
            sleep(1);

            // get connection status, current command will be handled in response
            printf("[>] ble_cmd_connection_get_status\n");
            ble_cmd_connection_get_status(0);
            if (wait_for_rsp() != APP_OK) {
                die();
            }


            // Connect to target with specific settings
            printf("[###] Connect to target [###]\n");
            printf("[>] ble_cmd_gap_connect_direct\n");
            ble_cmd_gap_connect_direct(app_connection.target.addr,
                                       app_connection.addr_type,
                                       app_connection.conn_interval_min,
                                       app_connection.conn_interval_max,
                                       app_connection.timeout,
                                       app_connection.latency);
            if (wait_for_rsp() != APP_OK) {
                die();
            }
            if (wait_for_evt() != APP_OK) {
                die();
            }

            // Are we connected?
            if (app_connection.state != APP_DEVICE_CONNECTED) {
                printf("[#] Connecting failed.\n");
                die();
            }


            uint8 serv_conf_enable[2] = { 0x01, 0x00 };
            uint8 serv_conf_len       = sizeof(serv_conf_enable);
            uint8 serv_conf_handle    = 19;


            printf("[###]Activate Service Notification by handle[###]\n");


            printf("[>] ble_cmd_attclient_attribute_write\n");
            ble_cmd_attclient_attribute_write(app_connection.handle,
                                              serv_conf_handle, serv_conf_len,
                                              serv_conf_enable);
            if (wait_for_rsp() != APP_OK) {
                die();
            }
            wait_for_evt();
        }



        // the thread function
        void threadedFunction() {

            printf("entered threadedFunction\n");;

            struct ble_header api_header;

            uint16_t acc_x, acc_y;
            uint16_t acc_x_max = 0;
            uint16_t acc_y_max = 0;
            uint16_t acc_x_min = 31000;
            uint16_t acc_y_min = 31000;


            while(isThreadRunning()) {

                acc_x = 0;
                acc_y = 0;

                //printf("thread is running\n");

                // read header of incoming ble message
                SimpleSerial::ReadBleHeader(api_header);

                if (SimpleSerial::ReadBleMessage(api_header)) {
                    printf("Error reading message\n");
                    exit(-1);
                }

                // Check if everything is fine
                if (issetFlag(app_state, APP_ATTCLIENT_ERROR)) {
                    die();
                };


//                for (int i = 0; i < 4; ++i)
//                    std::cout << std::hex << std::setfill('0') << std::setw(2) << 0u + app_attclient.value.data[i] << " ";
//                std::cout << "\n";

                // X
                acc_x = (acc_x << 8) + app_attclient.value.data[1];
                acc_x = (acc_x << 8) + app_attclient.value.data[0];
                if(acc_x_max < acc_x)
                    acc_x_max = acc_x;
                if(acc_x_min > acc_x)
                    acc_x_min = acc_x;

                // y
                acc_y = (acc_y << 8) + app_attclient.value.data[3];
                acc_y = (acc_y << 8) + app_attclient.value.data[2];
                if(acc_y_max < acc_y)
                    acc_y_max = acc_y;
                if(acc_y_min > acc_y)
                    acc_y_min = acc_y;

//                std::cout << std::dec;
//                std::cout << "acc_x: " << acc_x << std::endl;
//                std::cout << "acc_y: " << acc_y /*<< "     acc_y*0.75=" << (acc_y*0.75)*/ << std::endl;
//                std::cout << "acc_x_min: " << acc_x_min << std::endl;
//                std::cout << "acc_y_min: " << acc_y_min << std::endl;
//                std::cout << "acc_x_max: " << acc_x_max << std::endl;
//                std::cout << "acc_y_max: " << acc_y_max << std::endl << std::endl;


                static ofxBleAcceleratorEvent newEvent;
                newEvent.acc_x = (acc_x >> 6);
                newEvent.acc_y = (acc_y >> 6);

//                std::cout << "acc_x: " << newEvent.acc_x << std::endl;
//                std::cout << "acc_y: " << newEvent.acc_y /*<< "     acc_y*0.75=" << (acc_y*0.75)*/ << std::endl << std::endl;

                ofNotifyEvent(ofxBleAcceleratorEvent::events, newEvent);
            }


            // ... then disconnect
            printf("[###]Disconnect from target[###]\n");
            printf("[>] ble_cmd_connection_disconnect\n");
            ble_cmd_connection_disconnect(app_connection.handle);
            if (wait_for_rsp() != APP_OK) {
                die();
            }
            wait_for_evt();
        }



        int8 wait_for_rsp() {
            ble_header api_header;

            setFlag(app_state, APP_COMMAND_PENDING);

            while (issetFlag(app_state, APP_COMMAND_PENDING)) {
                SimpleSerial::ReadBleHeader(api_header);
                if (SimpleSerial::ReadBleMessage(api_header)) {
                    printf("Error reading message\n");
                    return APP_FAILURE;
                }
            }
            return APP_OK;
        }



        int8 wait_for_evt() {
            ble_header api_header;

            setFlag(app_state, APP_ATTCLIENT_PENDING);

            while (issetFlag(app_state, APP_ATTCLIENT_PENDING)) {
                SimpleSerial::ReadBleHeader(api_header);
                if (SimpleSerial::ReadBleMessage(api_header)) {
                    printf("Error reading message\n");
                    return APP_FAILURE;
                }
            }
            return APP_OK;
        }



        void die() {
            printf("Failure. End of program...\n");
            exit(-1);
        }



    private:
        const char* serial_one;
        // Storage to hold recently received data
        uint8 value_buffer[256] = { 0 };
};

#endif //_OFXBLE_LINUX_BLE112_H
