/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Qingyue Long , Kyle Lemmel 
 * User name: qinglong, klemmel
 * Maintainer: 
 * Created: 09/15/2016
 * Last-Updated: 09/21/2016
 *           By: qingyue long
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <stdlib.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  // initialize
  f3d_uart_init();
  // prevent the library from buffing I/O
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  while(1){
    // putchar('a');
    //  putstring("hello world");
   
    // putchar(getchar());
  int character_count = 1;
  int line_count = 1;
  int word_count = 1;
  int c;
  while ((c = getchar()) != 0x1b) {
      putchar(getchar());
    character_count++;
    if (c == '\n' || c == ' ') {
      word_count++;
    }

    if(c == '\n') {
      line_count++;
    }
  }
  printf("\n%d " ,line_count);
  printf("%d " ,word_count+1);
  printf("%d \n",character_count);
      
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
