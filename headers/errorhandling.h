#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>

void errhandle(int err) {
    if (err == LIBUSB_SUCCESS) return;

    switch (err)
    {
        case LIBUSB_ERROR_IO:
            printf("An error occurred: LIBUSB_ERROR_IO: Input/output error. Aborting.\n");
            break;

        case LIBUSB_ERROR_INVALID_PARAM:
            printf("An error occurred: LIBUSB_ERROR_INVALID_PARAM: Invalid parameter. Aborting.\n");
            break;

        case LIBUSB_ERROR_ACCESS:
            printf("An error occurred: LIBUSB_ERROR_ACCESS: Access denied (insufficient permissions). Try running as root. Aborting.\n");
            break;

        case LIBUSB_ERROR_NO_DEVICE:
            printf("An error occurred: LIBUSB_ERROR_NO_DEVICE: No such device (it may have been disconnected). Aborting.\n");
            break;

        case LIBUSB_ERROR_NOT_FOUND:
            printf("An error occurred: LIBUSB_ERROR_NOT_FOUND: Entity not found. Aborting.\n");
            break;

        case LIBUSB_ERROR_BUSY:
            printf("An error occurred: LIBUSB_ERROR_BUSY: Resource busy. Aborting.\n");
            break;

        case LIBUSB_ERROR_TIMEOUT:
            printf("An error occurred: LIBUSB_ERROR_TIMEOUT: Operation timed out. Aborting.\n");
            break;

        case LIBUSB_ERROR_OVERFLOW:
            printf("An error occurred: LIBUSB_ERROR_OVERFLOW. Aborting.\n");
            break;

        case LIBUSB_ERROR_PIPE:
            printf("An error occurred: LIBUSB_ERROR_PIPE. Aborting.\n");
            break;

        case LIBUSB_ERROR_INTERRUPTED:
            printf("An error occurred: LIBUSB_ERROR_INTERRUPTED: System call interrupted (perhaps due to signal). Aborting.\n");
            break;

        case LIBUSB_ERROR_NO_MEM:
            printf("An error occurred: LIBUSB_ERROR_NO_MEM: Insufficient memory. Aborting.\n");
            break;

        case LIBUSB_ERROR_NOT_SUPPORTED:
            printf("An error occurred: LIBUSB_ERROR_NOT_SUPPORTED: Operation not supported or unimplemented on this platform. Aborting.\n");
            break;

        case LIBUSB_ERROR_OTHER:
            printf("An error occurred: LIBUSB_ERROR_OTHER: Unknown error. Aborting.\n");
            break;
    }

    //TODO what about cleanup functions?

    exit(err);
}