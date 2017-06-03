/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Qingyue Long(qinglong), Hunter Johnson(hunjohns) 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
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

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct bmppixel {
// little endian byte order
  uint8_t b;
  uint8_t g;
  uint8_t r;
};

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}



int main(void) { 
  struct bmppixel PixelData[128];
  uint16_t Color[128];
  uint16_t RGB[3];
  FATFS Fatfs;		/* File system object */
  FIL Fil;		/* File object */
  BYTE Buff[128];		/* File read buffer */
  char footer[20];
  char filename[20];
  int count=0;
  int i,j;

  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_delay_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

    
 
  nunchuk_t data;
  int screen = 0; //between 0 and 3
  int num_screens = 3;
  float pitch;
  float roll;
  float accel_data[3];
  uint8_t madctl;

  while (1){

    f3d_nunchuk_read(&data);
    f3d_accel_read(accel_data);
    pitch = atanf(accel_data[0] / sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2)));
    roll = atanf(accel_data[1] / sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2)));
    if(pitch>0.5){
      madctl = 0x4;
    }
    if(pitch<-0.5){
      madctl = 0x2;
    }
    if(roll>0.5){
      madctl =0x7;
    }
    if(roll<-0.5){
      madctl = 0x1;
	}
      
    if(data.z == 1){
      screen++;
      while(data.z ==1){
	f3d_nunchuk_read(&data);
      }
      screen %= num_screens;
      f3d_lcd_fillScreen2(WHITE);
    }
    if(data.c == 1){
      screen = screen +2;
      while(data.c ==1){
	f3d_nunchuk_read(&data);
      }
      screen %= num_screens;
      f3d_lcd_fillScreen2(WHITE);
    }
    if(screen==0){
      strcpy(filename, "1.bmp");
    }
    if(screen==1){
      strcpy(filename, "2.bmp");
    }
    if(screen==2){
      strcpy(filename, "3.bmp");
    }
    rc = f_open(&Fil, filename, FA_READ);

    f3d_lcd_setAddrWindow(0,0,127,159,madctl);

    if (rc) die(rc);
    for (j=0; j<160;j++){
      rc = f_read(&Fil, PixelData, sizeof PixelData, &br);
      if (rc) die(rc);
      for(i=0; i<128;i++){
	RGB[0]=((uint16_t)(PixelData[i].r)) >>3;
	RGB[1]=((uint16_t)(PixelData[i].g)) >>2;
	RGB[2]=((uint16_t)(PixelData[i].b)) >>3;
	Color[i] =(uint16_t) ((RGB[0]<<11)|(RGB[1]<<5)|(RGB[2])); 
      }
      f3d_lcd_pushColor(Color,128);
    }
      rc = f_close(&Fil);
    if (rc) die(rc);
    
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
