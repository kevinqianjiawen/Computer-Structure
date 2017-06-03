/* main.c --- 
 * 
 *Author: Qingyue Long, Felli, Christopher James
 *User:qinglong, cfelli
 *Date Created: 9/29/16
 *Last Modified by: Qingyue Long
 *Date Last Modified: 10/5/16
 *Assignment: Lab 6
 */

//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main() {
	// Initialize
	f3d_led_init();
	f3d_uart_init();

	// Loop
	while(1){
		// Turn on and delay...
		f3d_led_all_on();
		delay();

		// Putting it here in the middle...
		printf("Hello world...\n");

		// Turn off and delay...
		f3d_led_all_off();
		delay();
	}
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
