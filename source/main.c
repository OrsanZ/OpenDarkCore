#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/color.h"
#include "../headers/enums.h"
#include "../headers/shared.h"
#include "../headers/errorhandling.h"

libusb_context *ctx = NULL;
libusb_device_handle *tHandle = NULL;

int main(int arc, char* argv[]) {
    char status = 0;
    char devicestatus = 0;

    color c1;
    color c2;

    char zones = -1;
    char speed = -1;
    char mode = -1;
    char random = 0;

    short dpi = -1;
    char dpi_profile = -1;
    char dpi_profile_mode = DARK_CORE_DPI_PROFILE_OVERRIDE; 

    for (size_t i = 1; argv[i] != NULL; i++)
    {
        if (strcmp(argv[i], "--help") == 0) 
        {
            printf(
                "OpenDarkCore - A linux CLI for Corsair's Dark Core\n"
                "\n"
                "--help - Display this helper text\n"
                "\n"
                "--static <RRR> <GGG> <BBB> - Sets the color to static.\n"
                "    Requires 3 values from 0-255 for red, green and blue.\n"
                "\n"
                "--rainbow - Sets the color mode to rainbow.\n"
                "    Requires no parameters.\n"
                "\n"
                "--pulse [random|RRR GGG BBB RRR GGG BBB] - Sets the color mode to pulse.\n"
                "    Requires either the parameter random, or 6 values from 0-255 for both colors.\n"
                "\n"
                "--shift [random|RRR GGG BBB RRR GGG BBB]- Sets the color mode to shift.\n"
                "    Requires either the parameter random, or 6 values from 0-255 for both colors.\n"
                "\n"
                "--dpi <profile> <dpi> - Sets the DPI of the mouse\n"
                "    Requires the DPI profile, 0 for Sniper, 1 for the first, 2 for the second, 3 for the third,\n"
                "    and the DPI value, from 100 to 16000.\n "
                "\n"
                "[Experimental]\n"
                "--zones - Specifies the mouse zone to be addressed by the lighting. Works similarly to chmod.\n"
                "Add 1 if you want to address the mouse's back,\n"
                "add 2 if you want to address the the thumb zone,\n"
                "add 4 if you want to address the scrollwheel.\n"
                "\n"
            );
            
            return 0;
        }
        else if (strcmp(argv[i], "--static") == 0)
        {
            if(mode != -1) {
                printf("Mode specified twice. Aborting.\n");
                return 0;
            }

            speed = 0;

            for (size_t j = 1; j < 4; j++)
            {
                if (argv[i+j] == NULL) {
                    printf("Expected further parameters. Aborting.\n");
                    return 0;
                } 

                //Tests if value is something other than a number.
                if (atoi(argv[i+j]) == 0 && strcmp(argv[i+j],"0") != 0) {
                    printf("Parameter %i was unexpected. Aborting.\n", i+j);
                    return 0;
                }
            }

            i++;
            c1.red = atoi(argv[i]);
            c2.red = atoi(argv[i]);

            i++;
            c1.green = atoi(argv[i]);
            c2.green = atoi(argv[i]);

            i++;
            c1.blue = atoi(argv[i]);
            c2.blue = atoi(argv[i]);

            mode = DARK_CORE_MODE_STATIC;
        }
        else if (strcmp(argv[i], "--rainbow") == 0)
        {
            if(mode != -1) {
                printf("Mode specified twice. Aborting.\n");
                return 0;
            }

            c1.red = 255;
            c2.red = 255;
            speed = 2;

            mode = DARK_CORE_MODE_RAINBOW;
        }
        else if (strcmp(argv[i], "--shift") == 0 || strcmp(argv[i], "--pulse") == 0)
        { 
            if(mode != -1) {
                printf("Mode specified twice. Aborting.\n");
                return 0;
            }

            if (strcmp(argv[i], "--pulse") == 0)
            {
                mode = DARK_CORE_MODE_PULSE;
            }
            else
            {
                mode = DARK_CORE_MODE_SHIFT;
            }

            i++;
            if (strcmp(argv[i], "random") == 0) {
                c1.red, c2.red = 255;
                c1.green, c2.green = 0;
                c1.blue, c2.blue = 0;

                random = 1;
            }
            else
            {
                for (size_t j = 1; j < 7; j++)
                {
                    if (argv[i+j] == NULL) {
                        printf("Expected further parameters. Aborting.\n");
                        return 0;
                    } 

                    //Tests if value is something other than a number.
                    if (atoi(argv[i+j]) == 0 && strcmp(argv[i+j],"0") != 0) {
                        printf("Parameter %i was unexpected. Aborting.\n", i+j);
                        return 0;
                    }
                }
                
                random = 3;

                c1.red = atoi(argv[i]);

                i++;
                c1.green = atoi(argv[i]);

                i++;
                c1.blue = atoi(argv[i]);

                i++;
                c2.red = atoi(argv[i]);

                i++;
                c2.green = atoi(argv[i]);

                i++;
                c2.blue = atoi(argv[i]);
            }
        }
        else if(strcmp(argv[i], "--speed") == 0)
        {
            i++;
            if (argv[i] == NULL) 
            {
                printf("Expected further parameters. Aborting.\n");
                return 0;
            } 

            if ((atoi(argv[i]) < 1) || (atoi(argv[i]) > 3))
            {
                printf("Speed can only be a value from 1 to 3. Aborting.\n");
                return 0;
            }   

            speed = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "--zones") == 0)
        {
            if (zones != -1)
            {
                printf("Zones specified twice. Aborting.\n");
                return 0;
            }

            i++;
            zones = atoi(argv[i]);
        }
        else if(strcmp(argv[i], "--dpi") == 0)
        {
            if(mode != -1) {
                printf("Mode specified twice. Aborting.\n");
                return 0;
            }

            for (size_t j = 1; j < 3; j++)
            {
                if (argv[i+j] == NULL) {
                    printf("Expected further parameters. Aborting.\n");
                    return 0;
                }
            }

            //Tests if value is something other than a number.
            if (atoi(argv[i+2]) == 0 && strcmp(argv[i+2], "0") != 0) {
                printf("Parameter %i was unexpected. Aborting.\n", i+2);
                return 0;
            }

            mode = DARK_CORE_MODE_DPI;

            i++;
            dpi_profile = atoi(argv[i]);

            if (dpi_profile > 4)
            {
                printf("DPI profiles are only valid from 0 to 3. Aborting.\n");
                return 0;
            }

            //TODO feature change thing
            i++;
            if (strcmp(argv[i], "on") == 0)
            {
                dpi_profile_mode = DARK_CORE_DPI_PROFILE_ON;
            } else if(strcmp(argv[i], "off") == 0)
            {
                dpi_profile_mode = DARK_CORE_DPI_PROFILE_OFF;
            }
            else
            {
                dpi = atoi(argv[i]);
                if (mode == DARK_CORE_MODE_DPI && ((dpi < 100) || (dpi > 16000))) {
                    printf("DPI is only valid from 100 to 16000.\n");
                    return 0;
                }
            }
        }
    }

    //Section to check for improper parameters
    if (mode == -1) {
        printf("Operation mode not specified.\n");
        return 0;
    } else if (mode == DARK_CORE_MODE_STATIC && speed != 0) {
        printf("Speed option is invalid when used with static mode %i.\n");
        return 0;
    }

    libusb_init(&ctx);

    libusb_device **list;
    libusb_device *tDevice = NULL;

    ssize_t cnt = libusb_get_device_list(ctx, &list);
    if (cnt == 0)
    {
        printf("Could not retrieve any USB devices. Aborting.\n");
        return 0;
    }

    for (ssize_t i = 0; i < cnt; i++) 
    {
        libusb_device *device = list[i];
        libusb_device_handle *handle;

        devicestatus = libusb_open(device, &handle);
        if (devicestatus != LIBUSB_SUCCESS)
        {
            printf("Error while opening a device, skipping. Code: %i\n", devicestatus);
            continue;
        }
        
        struct libusb_device_descriptor desc;
        devicestatus = libusb_get_device_descriptor(device, &desc);
        if (devicestatus != LIBUSB_SUCCESS)
        {
            printf("Error while getting a device descriptor, skipping. Code: %i\n", devicestatus);
            continue;
        }

        char manufacturer[256];
        devicestatus = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, manufacturer, 256);
        if (devicestatus < 0)
        {
            printf("Error while retrieving descriptor string, skipping. Code: %i\n", devicestatus);
            continue;
        }

        if (strcmp(manufacturer, "Corsair") != 0)
        {
            libusb_close(handle);
            continue;
        }

        char product[256];
        status = libusb_get_string_descriptor_ascii(handle, desc.iProduct, product, 256);
        if (devicestatus < 0)
        {
            printf("Error while retrieving descriptor string, skipping. Code: %i\n", devicestatus);
            continue;
        }


        if(
            strcmp(product, "CORSAIR DARK CORE RGB SE Wireless USB Receiver") == 0 
            || 
            strcmp(product, "CORSAIR DARK CORE RGB SE Gaming Mouse") == 0
        )
        {
            printf("FOUND: %s\n", product);
            tDevice = device;
            tHandle = handle;
            break;
        }

        libusb_close(handle);
    }

    libusb_free_device_list(list, 0);

    if(tDevice == NULL || tHandle == NULL)
    {
        libusb_exit(ctx);
        return 0;
    }
    //Not error handled because it is assumed that if this errors, there will be no kernel driver
    libusb_detach_kernel_driver(tHandle, 1);

    status = libusb_claim_interface(tHandle, 1);
    errhandle(status);

    struct libusb_config_descriptor* tConfig;
    status = libusb_get_active_config_descriptor(tDevice, &tConfig);
    errhandle(status);

    char endpoint = tConfig->interface[1].altsetting->endpoint[1].bEndpointAddress;

    unsigned char data[16];

    if (mode > 0)
    {
        memcpy(data, (char[16]){0x07, 0xAA, 0x00, 0x00, zones, mode, speed, random, 0x64, c1.red, c1.green, c1.blue, c2.red, c2.green, c2.blue, 0x05}, 16);
    } 
    else if(mode == DARK_CORE_MODE_DPI) 
    {
        char dpi1 = (dpi << 8) >> 8;
        char dpi2 = dpi >> 8;
        memcpy(data, (char[16]){0x07, 0x13, 0xD0 + dpi_profile, 0x00, 0x00, dpi1, dpi2, dpi1, dpi2, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00}, 16);
    }

    //Debug the data sent to the mouse
    /* 
    for (size_t i = 0; i < sizeof(data); i++) {
        printf("%i ", data[i]);
    } 
    printf("\n");
    */

    status = libusb_bulk_transfer(tHandle, endpoint, data, 16, NULL, 0);
    errhandle(status);

    libusb_attach_kernel_driver(tHandle, 1);

    libusb_release_interface(tHandle, 1);
    libusb_close(tHandle);
    libusb_exit(ctx);

    return 0;
}