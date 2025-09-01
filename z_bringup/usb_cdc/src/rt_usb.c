#include "rt_usb.h"
#include "sample_usbd.h"

LOG_MODULE_REGISTER(rt_usb);

static struct usbd_context *sample_usbd;
K_SEM_DEFINE(dtr_sem, 0, 1);


static inline void print_baudrate(const struct device *dev)
{
//	uint32_t baudrate;
//	int ret;
//
//	ret = uart_line_ctrl_get(dev, UART_LINE_CTRL_BAUD_RATE, &baudrate);
//	if (ret) {
//		LOG_WRN("Failed to get baudrate, ret code %d", ret);
//	} else {
//		LOG_INF("Baudrate %u", baudrate);
//	}
}


static void sample_msg_cb(struct usbd_context *const ctx, const struct usbd_msg *msg)
{
	LOG_INF("USBD message: %s", usbd_msg_type_string(msg->type));

	if (usbd_can_detect_vbus(ctx)) {
		if (msg->type == USBD_MSG_VBUS_READY) {
			if (usbd_enable(ctx)) {
				LOG_ERR("Failed to enable device support");
			}
		}

		if (msg->type == USBD_MSG_VBUS_REMOVED) {
			if (usbd_disable(ctx)) {
				LOG_ERR("Failed to disable device support");
			}
		}
	}
}


int32_t rt_usb__init()
{

    int err;

	sample_usbd = sample_usbd_init_device(sample_msg_cb);
	if (sample_usbd == NULL) {
		LOG_ERR("Failed to initialize USB device");
		return -ENODEV;
	}

	if (!usbd_can_detect_vbus(sample_usbd)) {
		err = usbd_enable(sample_usbd);
		if (err) {
			LOG_ERR("Failed to enable device support");
			return err;
		}
	}

	LOG_INF("USB device support enabled");

	return 0;
}