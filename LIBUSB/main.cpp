#include <libusb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <cstdint>


#define VENDOR_ID 0xbbaa
#define PRODUCT_ID 0xddcc

// Data transfer size for low-speed USB devices is 8 bytes,
// Data transfer size for high-speed USB devices is 64 bytes
#define TRANSFER_SIZE 8

//write_to_safety (the LED Light)
//LIBUSB_ENDPOINT_OUT = OUT: host to device

struct libusb_device_handle *device_handle = NULL;

struct __attribute__((packed)) health_t {
    uint32_t uptime_pkt;
    int voltage_pkt;
    uint32_t current_pkt;
    uint32_t can_rx_errs_pkt;
    uint32_t can_send_errs_pkt;
    uint32_t can_fwd_errs_pkt;
    uint32_t gmlan_send_errs_pkt;
    uint32_t faults_pkt;
    uint8_t ignition_line_pkt;
    uint8_t ignition_can_pkt;
    uint8_t controls_allowed_pkt;
    uint8_t gas_interceptor_detected_pkt;
    uint8_t car_harness_status_pkt;
    uint8_t usb_power_mode_pkt;
    uint8_t safety_mode_pkt;
    int16_t safety_param_pkt;
    uint8_t fault_status_pkt;
    uint8_t power_save_enabled_pkt;
    uint8_t heartbeat_lost_pkt;
    uint16_t alternative_experience_pkt;
    uint32_t blocked_msg_cnt_pkt;
};

int write_to_safety(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned int timeout){
    int err;
    const uint8_t bmRequestType = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE;
    err = libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, NULL, 0, timeout);
    return err;
}

int read_from_safety(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout){
    int err;
    const uint8_t bmRequestType = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE;
    err = libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
    return err;
}

bool send_heartbeat(bool Heartbeat_YesOrNo){
    return write_to_safety(0xf3, Heartbeat_YesOrNo, 0, 0);
}

int enable_green_light(uint16_t green_light){
    return write_to_safety(0xf7, green_light, 0, 0);
}

int set_fan_power(uint16_t fan_power){
    return write_to_safety(0xb1,fan_power, 0, 0);
}


//declaring our libusb USB device handle will be used for all of our interactions with the
//      - USB device via libusb (opening, closing, reading and writing commands)
//      - initialize the libusb library and check the returned result.
//      - setting our debugging output to the max level (Assuming it's for a wider range of debugging output)
// argc (argument count) is an integer that indicates how many arguments
//      - entered on the command line when the program was started
//argv (argument vector), is an array of pointers to arrays of character objects.
// -1 was inserted to inform user if there's a possibility of an error throughout this code

int main( int argc, char **argv){
    // Initialize libusb
    int result = libusb_init(NULL);
    if (result < 0) {
        printf("Error initializing libusb: %s\n", libusb_error_name(result));
        exit(-1);
    }
    // Set debugging output to max_level
    libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);


// If initialisation = successful:
//      - attempt to open the connected USB device
//          - matches with the vendor and product ID
// Open our ADU (Analog to digital unit device) that matches our vendor id and product id
    device_handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
    if (!device_handle) {
        printf("Error finding USB device \n");
        libusb_exit(NULL);
        exit(-2);
    }
    // In this block, we need to enable auto-detaching of the kernel driver
// In order to communicate, each USB device have endpoints. These endpoints act like pipes, in these pipes
// all the data are travelling.
// One endpoint have only one direction and can only be use just by 1 driver
// So you need to detach the kernel driver in order to make these endpoints available.
// A Linux kernel driver is a specific type of program that allows hardware and software to work together to accomplish a task
    libusb_set_auto_detach_kernel_driver(device_handle, 1);

// claim interface 0 on the device, defined with an IN and OUT endpoint used for reading and writing, respectively.
    result = libusb_claim_interface( device_handle, 0);
    if (result < 0)
    {
        printf("Error claiming interface: %s\n", libusb_error_name(result));
        if (device_handle){
            libusb_close(device_handle);
        }
        libusb_exit(NULL);
        exit(-3);
    }
    if (result >= 0){
        printf("you're connected hehe \n");
    }
    if (enable_green_light(1) < 0){
        printf("there's an error turning on the green light\n");
    }
    else{
        printf("there's a green light\n");
    }
    if (set_fan_power(100) < 0){
        printf("there's an error switching on the fan\n");
    }
    else{
        printf("the fan's spinning.\n");
    }

    health_t health;
    int err = read_from_safety(0xd2, 0, 0, (unsigned char*)&health, sizeof(health), 0);

    printf("The value for voltage packet is %d\n", health.voltage_pkt);
    printf("The value for uptime is %d\n", health.uptime_pkt);
    printf("Error code is %s\n", libusb_error_name(err));

    int heartbeat_count = 0;
    while(true){
        send_heartbeat(true);
        set_fan_power(100);
        heartbeat_count += 1;
        printf("There's a heartbeat sending to the STM Board, heartbeat = %d\n", heartbeat_count);
    }
    if (send_heartbeat(false)){
        printf("the device is not powered");
    }
}
