#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <argp.h>

#include "../headers/color.h"
#include "../headers/constants.h"
#include "../headers/enums.h"
#include "../headers/errorhandling.h"
#include "../headers/argparser.h"

/* Libusb Globals */
libusb_context *ctx = NULL;
libusb_device *target_device = NULL;
libusb_device_handle *target_handle = NULL;

/* Error handling globals */
int16_t usb_errno = 0;
int16_t usb_verboseno = 0;
int8_t kernel_detached = 0;

int main(int argc, char *argv[])
{
    struct arguments arguments;

    /* Initialize default values */
    arguments.verbose = 0;
    arguments.color1.red = 255;
    arguments.color2.red = 255;
    arguments.color1.green = 0;
    arguments.color2.green = 0;
    arguments.color1.blue = 0;
    arguments.color2.blue = 0;
    arguments.zones = 7;
    arguments.speed = DARK_CORE_UNSPECIFIED;
    arguments.lighting_mode = DARK_CORE_UNSPECIFIED;
    arguments.random = 0;
    arguments.dpi = DARK_CORE_UNSPECIFIED;
    arguments.dpi_profile = 1;
    arguments.angle_snapping = DARK_CORE_UNSPECIFIED;
    arguments.lift_height DARK_CORE_UNSPECIFIED;

    /* Parse arguments */
    argp_parse(&parser, argc, argv, 0, 0, &arguments);

    /* Initialize libusb */
    libusb_init(&ctx);

    /* Define list */
    libusb_device **list;

    /* Check if any USB devices were found */
    ssize_t count = libusb_get_device_list(ctx, &list);
    if (count == 0)
    {
        printf("Could not retrieve any USB devices. Aborting.\n");
        exit(0);
    }

    /* Loops through all USB devices to find one with matching criteria */
    for (size_t i = 0; i < count; i++)
    {
        /* Get current device */
        libusb_device *device = list[i];
        libusb_device_handle *handle;

        /* Open current device */
        usb_verboseno = libusb_open(device, &handle);
        if (usb_verboseno != LIBUSB_SUCCESS)
        {
            if (arguments.verbose)
                printf("Error while opening a device, skipping. Code: %i\n", usb_verboseno);
            continue;
        }

        /* Get device descriptor */
        struct libusb_device_descriptor desc;
        usb_verboseno = libusb_get_device_descriptor(device, &desc);
        if (usb_verboseno != LIBUSB_SUCCESS)
        {
            if (arguments.verbose)
                printf("Error while getting a device descriptor, skipping. Code: %i\n", usb_verboseno);
            continue;
        }

        /* If the vendor ID doesn't match, or none of the product IDs, then close the handle and skip */
        if (desc.idVendor != DARK_CORE_VENDOR_ID || !(desc.idProduct == DARK_CORE_WIRED_PRODUCT_ID || desc.idProduct == DARK_CORE_WIRELESS_PRODUCT_ID || desc.idProduct == DARK_CORE_SE_WIRED_PRODUCT_ID || desc.idProduct == DARK_CORE_SE_WIRELESS_PRODUCT_ID))
        {
            libusb_close(handle);
            continue;
        }

        /* Print name of found device if verbose */
        if (arguments.verbose)
        {
            /* Get product name in ASCII */
            char product_name[256];
            usb_verboseno = libusb_get_string_descriptor_ascii(handle, desc.iProduct, product_name, 256);
            if (usb_verboseno != 0)
            {
                printf("Error while retrieving descriptor string, skipping. Code: %i\n", usb_verboseno);
            }
            else
            {
                printf("FOUND: %s\n", product_name);
            }
        }

        /* Set device as target device and quit loop */
        target_device = device;
        target_handle = handle;
        break;
    }

    /* Free devices list */
    libusb_free_device_list(list, 0);

    /* Quit, if no device was found */
    if (target_device == NULL || target_handle == NULL)
    {
        printf("No device found. Please make sure that you have installed the udev rule, or alternatively, run as root (not recommended).\n");
        libusb_exit(ctx);
        exit(0);
    }

    /* If a kernel driver was active, detach it. */
    if (libusb_kernel_driver_active(target_handle, DARK_CORE_INTERFACE_NUM))
    {
        usb_errno = libusb_detach_kernel_driver(target_handle, DARK_CORE_INTERFACE_NUM);
        kernel_detached = 1;
    }

    /* Claim interface */
    usb_errno = libusb_claim_interface(target_handle, DARK_CORE_INTERFACE_NUM);
    errhandle();

    /* Transfer data to mouse, concerning lighting */
    if (arguments.lighting_mode != DARK_CORE_UNSPECIFIED)
    {
        uint8_t data[64] = {0x07, 0xAA, 0x00, 0x00, arguments.zones, arguments.lighting_mode, arguments.speed, arguments.random, 0x64, arguments.color1.red, arguments.color1.green, arguments.color1.blue, arguments.color2.red, arguments.color2.green, arguments.color2.blue, 0x05};
        usb_errno = libusb_bulk_transfer(target_handle, DARK_CORE_ENDPOINT_OUT_ADDRESS, data, sizeof(data), NULL, 0);
        errhandle();
    }

    /* Transfer data to mouse, concerning DPI */
    if (arguments.dpi != DARK_CORE_UNSPECIFIED)
    {
        /* Flip high and low byte of DPI */
        int8_t dpi1 = (arguments.dpi << 8) >> 8;
        int8_t dpi2 = arguments.dpi >> 8;

        uint8_t data[64] = {0x07, 0x13, 0xD0 + arguments.dpi_profile, 0x00, 0x00, dpi1, dpi2, dpi1, dpi2, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00};
        usb_errno = libusb_bulk_transfer(target_handle, DARK_CORE_ENDPOINT_OUT_ADDRESS, data, sizeof(data), NULL, 0);
        errhandle();
    }

    /* Enables/Disables angle snapping */
    if (arguments.angle_snapping != DARK_CORE_UNSPECIFIED)
    {
        uint8_t data[64] = {0x07, 0x13, 0x04, 0x00, arguments.angle_snapping, 0x05};

        usb_errno = libusb_bulk_transfer(target_handle, DARK_CORE_ENDPOINT_OUT_ADDRESS, data, sizeof(data), NULL, 0);
        errhandle();
    }

    /* Sets lift height */
    if (arguments.lift_height != DARK_CORE_UNSPECIFIED)
    {
        uint8_t data[64] = {0x07, 0x13, 0x03, 0x00, arguments.lift_height + 2};

        usb_errno = libusb_bulk_transfer(target_handle, DARK_CORE_ENDPOINT_OUT_ADDRESS, data, sizeof(data), NULL, 0);
        errhandle();
    }

    /* Release interface, close device and exit. */
    libusb_release_interface(target_handle, DARK_CORE_INTERFACE_NUM);

    /* Reattach kernel driver */
    if (kernel_detached)
        libusb_attach_kernel_driver(target_handle, DARK_CORE_INTERFACE_NUM);

    libusb_close(target_handle);
    libusb_exit(ctx);

    exit(0);
}