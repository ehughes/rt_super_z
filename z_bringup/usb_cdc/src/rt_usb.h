#ifndef RT_USB_H
#define RT_USB_H

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>



int32_t rt_usb__init();

#endif // RT_USB_H