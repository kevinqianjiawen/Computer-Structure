//main.c for lab6
/**********************************************************
 *lab5/main.c
 * 
 *These definitions work with gyro input and display
 *information to the leds and lcd
 *      *
 *Author: Qingyue Long, Jeremiah Weating
 *User:qinglong, jweating
 *Date Created: 9/22/16
 *Last Modified by: Qingyue Long
 *Date Last Modified: 9/28/16
 *Assignment: Lab 5
 *            */
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

int main(void){
  f3d_gyro_init();
  f3d_uart_init();
  f3d_led_init();
  f3d_user_btn_init();

  setvbuf(stdin, NULL, _IONBF, 0); //clearing the stdbuffers// 
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

 float floatArray[3];
  int i = 0;
  int axisvalue = 0;
  int axis = 0;
  float value = 0;
  int c;
  while(1){
    f3d_gyro_getdata(floatArray);
  //  value=floatArray[axis];
    f3d_led_all_off();
 
   
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != (uint16_t)RESET){
      if(c=getchar()=='x'){
       axisvalue=1; 
       axis=0;
      printf("axis: X\n");
      }
     else  if(c=getchar()=='y'){
       axisvalue=2;
       axis=1;
       printf("axis: Y\n");
     }
     else  if(c=getchar()=='z'){
       axisvalue=3;
       axis=2;
       printf("axis: Z\n");
     }
    }
     if(user_btn_read()){
       axis=axisvalue%3;   
       printf("axis: %c\n", axis+88);
       axisvalue++;
       while(user_btn_read());
    }
      
     value = floatArray[axis];
      if(value > 5){
        f3d_led_on(1);
        if(value > 50)
	  f3d_led_on(2);
	if(value > 100)
          f3d_led_on(3); 
         if(value > 150)
	  f3d_led_on(4);
	if(value > 200)
          f3d_led_on(5);      
      }
      else  {
        if(value< -5)
         f3d_led_on(1);
        if(value <-50)
	  f3d_led_on(0);
	if(value < -100)
          f3d_led_on(7); 
         if(value < -150)
	  f3d_led_on(6);
	if(value < -200)
          f3d_led_on(5);    
      }
      //  f3d_led_all_off();
    
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
