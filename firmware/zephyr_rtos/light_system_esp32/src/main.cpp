#include <light_sysESP32.h>

int main(void){

    	/* configure interrupt and callback to receive data */
	int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

	if (ret < 0) {
		if (ret == -ENOTSUP) {
			printk("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			printk("UART device does not support interrupt-driven API\n");
		} else {
			printk("Error setting UART callback: %d\n", ret);
		}
		return 0;
	}
	uart_irq_rx_enable(uart_dev);   
    gpio_pin_configure(gpio_dev, LED_WSB1_PIN, GPIO_OUTPUT);


	print_uart("Network AGS System Online.\r\n");

    k_timer_init(&top_timer, top_timer_cb, NULL);
  
    for(int i = 0; i < NUM_LEDS_STRIP_1; i++){
        led_strip[i].blue = 0;
        led_strip[i].green = 0;
        led_strip[i].red = 0;
    }

    



    return 0;
}