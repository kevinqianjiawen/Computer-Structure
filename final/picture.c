/* picture.c ---                                                                                                                                                                                           
 *                                                                                                                                                                                                         
 * Filename: picture.c                                                                                                                  
 * Description:                                                                                                                                                                                            
 * Author: Qingyue Long (qinglong) Jiawen Qian (jiawqian)                                                                                                                                                  
 * Maintainer:                                                                                                                                                                                             
 * Created: 11/17/2016                                                                                                                                                                                     
 * Last-Updated: 12/06/2016                                                                                                                                                                                
 *           By:                                                                             
 *     Update #: 0                                                                                                                                                                                         
 * Keywords:                                                                                                                                                                                               
 * Compatibility:                                                                                                                                                                                          
 *                                                                                                                                                                                                         
 */
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "picture.h"







void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}


//because lcd only read 16 bit data, we convert bmp struct to uint16_t
uint16_t convert_pixel(bmp pixel){
  uint16_t color;
  color = ((uint16_t) (pixel.r >> 3)) << 11 | ((uint16_t) (pixel.g >> 2)) << 5 | ((uint16_t) (pixel.b >> 3));
  return color;
}


//read the file and the get the portait or landscape of the image
void draw_image(int width, int height, uint16_t** a, int x, int y){
  int k;

  //read data and draw image
  f3d_lcd_setAddrWindow(x,y, width + x - 1, height + y - 1, 0x2);
  for(k = 0; k < height; k++){
    f3d_lcd_pushColor(a[k], width);
  }      
}




