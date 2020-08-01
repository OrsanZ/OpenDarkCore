#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/errorhandling.h"

/* Error handler, which automatically closes the libusb session */
void errhandle()
{
  if (usb_errno == LIBUSB_SUCCESS)
    return;

  printf("An error occured: %s\nDescription: %s\n", libusb_error_name(usb_errno), libusb_strerror(usb_errno));

  libusb_attach_kernel_driver(target_handle, 1);

  libusb_release_interface(target_handle, 1);
  libusb_close(target_handle);
  libusb_exit(ctx);

  exit(usb_errno);
}