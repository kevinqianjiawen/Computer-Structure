/* main.c ---
*
*Author: Qingyue Long, Felli, Christopher James
 *User:qinglong, cfelli
 *Date Created: 9/29/16
 *Last Modified by: Qingyue Long
 *Date Last Modified: 10/5/16
 *Assignment: Lab 6
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>

#define TIMER 20000

/*
void delay(int count) {
  while (count-- > 0) {
    int i = 10000; 
    while (i-- > 0) {
      asm("nop");
    }
  }
}
*/

int main(void) {
  f3d_gyro_init();
  f3d_lcd_init();

  float f[3];  char array0[50];
  char array1[50];
  char array2[50];
  

  f3d_lcd_fillScreen2(WHITE);
  
  while(1){
	// Gets the data
  	f3d_gyro_getdata(f);

	// Convert to character
        sprintf(array0, "%f", f[0]);
	sprintf(array1, "%f", f[1]);
	sprintf(array2, "%f", f[2]);

	// Draws it...
  	f3d_lcd_drawString(10, 10, array0, RED, WHITE);
	f3d_lcd_drawString(10, 30, array1, GREEN, WHITE);
	f3d_lcd_drawString(10, 50, array2, BLUE, WHITE);
	
	// Graphical Representation
	int i=0;
        int j=0;
  	float z=f[0];
        float z1=f[1];
        float z2=f[2];
        uint8_t y;

        //x
	//positive
        if(z>0 && z < ST7735_width){
	  uint16_t x[(int)z];
	for (y = 0; y <z; y++){
            x[y] = RED;
      	f3d_lcd_setAddrWindow (0,80,z-1,90,MADCTLGRAPHICS);
	  }
        for (y=0;y<10; y++) {
            f3d_lcd_pushColor(x, z);
         } 
	}

	//negative
	else if(z<0 && (-z) < ST7735_width){  
        uint16_t xx[(int)(-z)];

        for (y = 0; y < -z ; y++){
            xx[y] = RED;
	    f3d_lcd_setAddrWindow (ST7735_width+z, 80, ST7735_width,90,MADCTLGRAPHICS);
	  }
        for (y=0;y<10; y++) {
            f3d_lcd_pushColor(xx, -z+1);
         } 
      
	}
       
	//y
        //positive
         if(z1>0 && z1 < ST7735_width ){
	  uint16_t x1[(int)z1];
	for (y = 0; y <z1; y++)
            x1[y] = GREEN;
	f3d_lcd_setAddrWindow (0,100,z1-1,110,MADCTLGRAPHICS);
        for (y=0;y<10; y++) {
            f3d_lcd_pushColor(x1, z1);
        }
	}
        //negative
	else if(z1<0 && (-z1) < ST7735_width){  
        uint16_t xx1[(int)(-z1)];

        for (y = 0; y < -z1 ; y++){
            xx1[y] = GREEN;
	    f3d_lcd_setAddrWindow (ST7735_width+z1, 100, ST7735_width,110,MADCTLGRAPHICS);
	  }
        for (y=0;y<10; y++) {
            f3d_lcd_pushColor(xx1, -z1+1);
         } 
      
	}

	 //z
	 //positive
         if(z2>0 && z2 < ST7735_width){
	  uint16_t x2[(int)z2];
	for (y = 0; y <z2; y++)
            x2[y] = BLUE;
            f3d_lcd_setAddrWindow (0,120,z2-1,130,MADCTLGRAPHICS);
        for (y=0;y<10; y++) {
            f3d_lcd_pushColor(x2, z2);
        }
	}
        //negative
	else if(z2<0 && (-z2) < ST7735_width){  
        uint16_t xx2[(int)(-z2)];

        for (y = 0; y < -z2 ; y++){
            xx2[y] = BLUE;
	    f3d_lcd_setAddrWindow (ST7735_width+z2,120, ST7735_width,130,MADCTLGRAPHICS);
	  }
        for (y=0;y<10; y++) {
            f3d_lcd_pushColor(xx2, -z2+1);
         } 
      
	}
	 delay(150);
        uint16_t renew[ST7735_width];
        for (y = 0; y < ST7735_width; y++){
          renew[y] = WHITE;
        f3d_lcd_setAddrWindow (0,80,ST7735_width-1,160,MADCTLGRAPHICS);}
        for (y=0;y<61; y++) {
          f3d_lcd_pushColor(renew,ST7735_width);
	 // f3d_lcd_fillScreen2(WHITE);
       }

       }
       }
/*
        if(z>0){
	  for (i=70; i<81; i++){
	    for (j=0; j<z; j++){
               
	      f3d_lcd_drawPixel(j, i, RED);
	    }
	  }
 	}
        else{
           for (i=70; i<81; i++){
	    for (j=200; j>100+z; j--){
               
	      f3d_lcd_drawPixel(j, i, RED);
	    }
	  }

	}
          for (i= 70; i<81; i++){
            for(j=0; j< 200; j++){
              f3d_lcd_drawPixel(j, i, WHITE);
	    }
	  }
	  //y
	  if(z1>0){
          for (i=90; i<101; i++){
	    for (j=0; j<z1; j++){
               
	      f3d_lcd_drawPixel(j, i, GREEN);
	   }
	  }
	  }
         else{
           for (i=90; i<101; i++){
	    for (j=200; j>100+z1; j--){
               
	      f3d_lcd_drawPixel(j, i, GREEN);
	    }
	  }
	 }
          for (i= 90; i<101; i++){
            for(j=0; j< 200; j++){
              f3d_lcd_drawPixel(j, i, WHITE);
	    }
	  }
	  //z
	  if(z2>0){
          for (i=110; i<121; i++){
	    for (j=0; j<z2; j++){
               
	      f3d_lcd_drawPixel(j, i, BLUE);
	   }
	  }
          }
           else{
           for (i=110; i<121; i++){
	    for (j=200; j>100+z2; j--){
               
	      f3d_lcd_drawPixel(j, i, BLUE);
	    }
	   }
          }

          for (i= 110; i<121; i++){
            for(j=0; j< 200; j++){
              f3d_lcd_drawPixel(j, i, WHITE);
	    }
	    }

  }
  }
*/
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
