#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>
/* Author: Qingyue Long, Jiawen Qian
 * User name: qinglong, jiawqian
 * Date Created: 9/08/2016
 * Date Last Modified: 9/08/2016
 * part of: lab3
 */
// Simple looping delay function
void delay(int data) {
  int i = data;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_led_init();
  f3d_user_btn_init();
  int i;
  int j;
   while(1){
   f3d_led_all_off();
   while(user_btn_read());
   delay(2000000);
   for(i = 0; i<8; i++){
     f3d_led_on(i);
     while(user_btn_read());
     delay(2000000);
     }
   f3d_led_all_on();
   while(user_btn_read());
   delay(20000000);
   for(j = 0; j<8; j++){
     f3d_led_off(j);
     while(user_btn_read());
     delay(2000000);
   }
   }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
