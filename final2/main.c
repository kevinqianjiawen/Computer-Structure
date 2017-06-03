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
//airplane
uint16_t** airplane;
uint16_t** airplane2;
//enermy1
uint16_t** enermy1;
uint16_t** enermyWhite;
//draw image variable
FRESULT rc;
uint16_t color;
UINT bw, br;

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */


//airplane x,y
//nunchunk shift variable
nunchuk_t nundata;
int x = 30;
int y = 30;

int airplaneSize = 40;


//structure
bmpfile_magic magic;
bmpfile_header header;
BITMAPINFOHEADER info;

//border of lcd
int border_x = 128;
int border_y = 160;

static const uint16_t height_width_difference = ST7735_height - ST7735_width;


void truncateImageBottom();
void readHeaders();

void truncateImageBottom() {
  UINT br;
  bmp trashPixelBuffer[ST7735_width];
  int i;
  for (i = 0; i < height_width_difference; i++) {
    f_read(&fid, (void *) trashPixelBuffer, sizeof(bmp) * ST7735_width, &br);
  }
}

void readHeaders() {
  UINT br;
  // read bmp headers
  f_read(&fid, (void *) &magic, 2, &br);
  f_read(&fid, (void *) &header, sizeof(header), &br);
  f_read(&fid, (void *) &info, sizeof(info), &br);

  // @hack - we have to throw away a bunch of bytes here
  //         for the image to line up correctly
  const int TRASH_BYTES = sizeof(bmp) * 28;
  bmp  bmpTrash[TRASH_BYTES];
  f_read(&fid, (void *) bmpTrash, TRASH_BYTES, &br);
}


uint16_t** store_image(char file[], int height, int width){
  int k;
  int i;
  bmp data[width];
  uint16_t** plane;

  //assign space for airplane
  plane = malloc(height * sizeof *plane);
  for(i = 0; i < height; i++){
    plane[i] = malloc(height * sizeof *plane[i]);
  }

 
  f_mount(0, &Fatfs);

  //open file
  rc = f_open(&fid, file, FA_READ);
  if(rc) die(rc);


  readHeaders();
 
  //read data and draw image
  for(k = 0; k < height; k++){
    rc = f_read(&fid,(void *) &data, sizeof data, &br);
    // if(rc) die(rc);
    for (i = 0; i < width; i++) {
      if (rc || !br) break;
      color = convert_pixel(data[i]);
      plane[k][i] = color;
    }
  }
  //close the file
  rc = f_close(&fid);
  if(rc) die(rc);

  return plane;
  
} 


uint16_t** store_white(int height, int width){
  int k;
  int i;
  uint16_t** white;

  //assign space for airplane
  white = malloc(height * sizeof *white);
  for(i = 0; i < height; i++){
    white[i] = malloc(width * sizeof *white[i]);
  }

  //read data and draw image
  for(k = 0; k < height; k++){
    for (i = 0; i < width; i++) {
      white[k][i] = WHITE;
    }
  }

  return white;
  
} 

/*
uint16_t** changeColor(uint16_t** plane, int height, int width){
  int k;
  int i;
  uint16_t** planeWhite;
  planeWhite = plane;

  for(k = 0; k < height; k++){
    for (i = 0; i < width; i++) {
      planeWhite[k][i] = WHITE;
    }
  }
  return planeWhite;
}
*/
int main(void){
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  printf("1");
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
   int i;
   //assign space for airplane
   airplane = malloc(airplaneSize * sizeof *airplane);
   airplane2 = malloc(airplaneSize * sizeof *airplane2);
   for(i = 0; i < airplaneSize; i++){
    airplane[i] = malloc(airplaneSize * sizeof *airplane[i]);
    airplane2[i] = malloc(airplaneSize * sizeof *airplane2[i]);
   }


  printf("2");
  f3d_lcd_fillScreen(WHITE);
  printf("3");
  airplane = store_image("airplane.bmp", airplaneSize, airplaneSize);
  printf("4");
  int a;
  int b;
  for(x = 0; x < 40; x++){
    for (y = 0; y < 40; y++){
      printf("%d", airplane[x][y]);
    }
    }
  airplane2 = store_white(airplaneSize, airplaneSize);
 draw_image(airplaneSize, airplaneSize, airplane, 100, 100);
 delay(1000000);
  /*for(x = 0; x < 40; x++){
    for (y = 0; y < 40; y++){
      printf("%d", airplane2[x][y]);
    }
    }*/
  printf("5");
  while(1){
    f3d_nunchuk_read(&nundata);
    f3d_lcd_fillScreen(BLUE);
    for(a = 0; a < 40; x++){
      for (b = 0; b < 40; y++){
	printf("%d", airplane[a][b]);
      }
    }
   
    // f3d_lcd_fillScreen(RED);
  if(nundata.jx == 0){

    draw_image(airplaneSize, airplaneSize, airplane2, x, y);
    
    x--;
       
    if(x < 0){
      x = 0;
    }

    draw_image(airplaneSize, airplaneSize, airplane, x, y);
    
    
    
 
  }else if(nundata.jx == 255){
    

      draw_image(airplaneSize, airplaneSize, airplane2,x, y);
      x++;
      if(x > border_x - airplaneSize){
	x = 128- airplaneSize;
      }
      draw_image(airplaneSize, airplaneSize, airplane, x, y);
  

  
  }

  if(nundata.jy == 0){


      draw_image(airplaneSize, airplaneSize, airplane2, x, y);
      y--;
      if(y < 0){
	y =0;
      }
      draw_image(airplaneSize, airplaneSize, airplane, x, y);
    
  } else if(nundata.jy == 255){
    

      draw_image(airplaneSize, airplaneSize, airplane2,x,y);
      y++;
      if(y > border_y - airplaneSize){
	y = 160 - airplaneSize;
      }
      draw_image(airplaneSize, airplaneSize, airplane, x, y);
    
	
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
