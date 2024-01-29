#pragma once
#include <light_sys_timer_callbacks.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>
#include <vector>
#include <bitset>

#include <stdio.h>
// #include <sys/printk.h>

//gpio
#define LED_WSB1_PIN 14
#define NUM_LEDS_STRIP_1 5


#define MSG_SIZE 32


#include <string.h>
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);


const struct device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio1));

static const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));

static struct k_timer top_timer;

struct LED {
   //each LED has 3 8-bit values, following GRB
   //https://www.arrow.com/en/research-and-events/articles/protocol-for-the-ws2812b-programmable-led

   uint8_t green;
   uint8_t red;
   uint8_t blue;

};

volatile LED led_strip[NUM_LEDS_STRIP_1];


static char rx_buf[MSG_SIZE];
static int rx_buf_pos;
/*
    Print a null terminated string
*/
void print_uart(char *buf)
{
	int msg_len = strlen(buf);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}


/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 */
void serial_cb(const struct device *dev, void *user_data)
{
    //character, ASICI
	uint8_t c;

	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	/* read until FIFO empty */
	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
		if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {
			/* terminate string */
			rx_buf[rx_buf_pos] = '\0';

			/* if queue is full, message is silently dropped */
			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

			/* reset the buffer (it was copied to the msgq) */
			rx_buf_pos = 0;
		} else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
			rx_buf[rx_buf_pos++] = c;
		}
		/* else: characters beyond buffer size are dropped */
	}
}