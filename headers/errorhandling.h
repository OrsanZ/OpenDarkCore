#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

extern libusb_context *ctx;
extern libusb_device_handle *target_handle;
extern int16_t usb_errno;
extern int16_t usb_verboseno;
extern int8_t kernel_detached;

/* Handles error codes from usb_errno */
void errhandle();

#endif