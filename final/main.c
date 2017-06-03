/* main.c --- 
 * 
 * Filename: main.c
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

int start = 0;
//airplane
uint16_t** airplane;
uint16_t** airplane2;
int airplaneSize = 40;
int airplaneSpeed = 5;
int score = 0;
char scoreChar[50];
//enermy1
uint16_t** enermy1;
uint16_t** enermyWhite;
int enermy1_x;
int enermy1_y = 130;
int enermy1Speed = 2;
int enermy1Point = 50;
int enermy1Counter = 0;
//enermy2
uint16_t** enermy2;
uint16_t** enermy2White;
int enermy2_x;
int enermy2_y = 130;
int enermy2Speed = 3;
int enermy2Size = 40;
int enermy2Point =40;
int enermy2launch = 0;

//bullet
uint16_t** bullet;
uint16_t** bulletWhite;
int bulletSpeed = 3;
int bulletSize = 20;

//blood

int blood = 6;
uint16_t** bloodImage;
uint16_t** bloodWhite;
int bloodSize = 20;
int blood_x;
int blood_y = 200;
int bloodSpeed = 1;
int bloodlaunch = 0;

//background
int backHeight = 160;
int backWidth = 128;
//bloodbar
uint16_t bloodBar[5];
uint16_t bloodBarWhite[5];
int bar_x = 2;
int bar_y = 10;

#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;


struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};


struct ckhd hd;
uint32_t  waveid;
struct fmtck fck;


BYTE Buff[512];		/* File read buffer */
int ret;

int launch = 0;

int c;

struct bullet{
  int bullet_x;
  int bullet_y;
};
struct bullet list[5];

int counter = 0;



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

void drawBloodBar(int x, int y){
  int i;
  for(i =0; i < 5; i++){
    bloodBar[i] = RED;
  }
  f3d_lcd_setAddrWindow(x, y, x + 4, y - 5, 0x2);
  
  for(i = 0; i < 6; i++){
    f3d_lcd_pushColor(bloodBar, 5);
  }
}

void drawBloodBarWhite(int x, int y){
  int i;
 for(i =0; i < 5; i++){
    bloodBarWhite[i] = WHITE;
  }
  f3d_lcd_setAddrWindow(x, y, x + 4, y - 5, 0x2);
  for(i = 0; i < 6; i++){
    f3d_lcd_pushColor(bloodBarWhite, 5);
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
void readHeaders2() {
  UINT br;
  // read bmp headers
  f_read(&fid, (void *) &magic, 2, &br);
  f_read(&fid, (void *) &header, sizeof(header), &br);
  f_read(&fid, (void *) &info, sizeof(info), &br);

  // @hack - we have to throw away a bunch of bytes here
  //         for the image to line up correctly
  const int TRASH_BYTES = sizeof(bmp) * 1;
  bmp  bmpTrash[TRASH_BYTES];
  f_read(&fid, (void *) bmpTrash, TRASH_BYTES, &br);
}



void store_image(char file[], int height, int width){
  int k;
  int i;
  bmp data[width];
  


 
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
      airplane[k][i] = color;
      airplane2[k][i] = WHITE;
    }
  }
  //close the file
  rc = f_close(&fid);
  if(rc) die(rc);
} 

void store_enermy1(char file[], int height, int width){
  int k;
  int i;
  bmp data[width];
  

  
 
  f_mount(0, &Fatfs);

  //open file
  rc = f_open(&fid, file, FA_READ);
  if(rc) die(rc);


  readHeaders();
 
  //read data and draw image
  for(k = 0; k < height; k++){
    rc = f_read(&fid,(void *) &data, sizeof data, &br);
    for (i = 0; i < width; i++) {
      if (rc || !br) break;
      color = convert_pixel(data[i]);
      enermy1[k][i] = color;
      enermyWhite[k][i] = WHITE;
    }
  }
  //close the file
  rc = f_close(&fid);
  if(rc) die(rc);
} 

void store_enermy2(char file[], int height, int width){
  int k;
  int i;
  bmp data[width];
  


 
  f_mount(0, &Fatfs);

  //open file
  rc = f_open(&fid, file, FA_READ);
  if(rc) die(rc);


  readHeaders2();
 
  //read data and draw image
  for(k = 0; k < height; k++){
    rc = f_read(&fid,(void *) &data, sizeof data, &br);
    for (i = 0; i < width; i++) {
      if (rc || !br) break;
      color = convert_pixel(data[i]);
      enermy2[k][i] = color;
      enermy2White[k][i] = WHITE;
    }
  }
  //close the file
  rc = f_close(&fid);
  if(rc) die(rc);
}

void store_bullet(char file[], int height, int width){
  int k;
  int i;
  bmp data[width];
  


 
  f_mount(0, &Fatfs);

  //open file
  rc = f_open(&fid, file, FA_READ);
  if(rc) die(rc);


  readHeaders2();
 
  //read data and draw image
  for(k = 0; k < height; k++){
    rc = f_read(&fid,(void *) &data, sizeof data, &br);
    for (i = 0; i < width; i++) {
      if (rc || !br) break;
      color = convert_pixel(data[i]);
      bullet[k][i] = color;
      bulletWhite[k][i] = WHITE;
    }
  }
  //close the file
  rc = f_close(&fid);
  if(rc) die(rc);
}
void store_heart(char file[], int height, int width){
  int k;
  int i;
  bmp data[width];
  


 
  f_mount(0, &Fatfs);
  
  //open file
  rc = f_open(&fid, file, FA_READ);
  if(rc) die(rc);


  readHeaders2();
 
  //read data and draw image
  for(k = 0; k < height; k++){
    rc = f_read(&fid,(void *) &data, sizeof data, &br);
    for (i = 0; i < width; i++) {
      if (rc || !br) break;
      color = convert_pixel(data[i]);
      bloodImage[k][i] = color;
      bloodWhite[k][i] = WHITE;
    }
  }
  //close the file
  rc = f_close(&fid);
  if(rc) die(rc);
}




int collsion(int x1, int y1, int size1, int x2, int y2, int size2){
  if(x1 > x2 & x1 < (x2 + size1) & (y1 + size1) >= y2 & (y1+size1) < (y2 + 2 * size2)){
    return 1;
  }else if(x1 < x2 & (x1 + size1) > x2 & (y1 + size1) >= y2 & (y1+size1) < (y2 + 2 * size2)){
    return 1;
  }else{
    return 0;
  }  
}

int collsion_bullet(int x1, int y1, int size1, int x2, int y2, int size2){
  if(x1 > x2 & x1 < (x2 + size2) & y1 > y2 + 5 & y1 <= (y2 +8)){
    return 1;
  }else if(x1 < x2 & (x1 + size1) > x2 & y1 > y2 + 5 & y1 <= (y2 +8)){
    return 1;
  }else{
    return 0;
  }  
}

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}



int play_audio(char * filename){
    f_mount(0, &Fatfs);/* Register volume work area */

  printf("\nOpen thermo.wav\n");
  rc = f_open(&fid, filename, FA_READ);
  
  if (!rc) {
 
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    //audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
}

int main(void){
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
  f3d_timer2_init();
  f3d_dac_init();

  int i;
  //assign space for airplane
  airplane = malloc(airplaneSize * sizeof *airplane);
  airplane2 = malloc(airplaneSize * sizeof *airplane2);
  for(i = 0; i < airplaneSize; i++){
    airplane[i] = malloc(airplaneSize * sizeof *airplane[i]);
    airplane2[i] = malloc(airplaneSize * sizeof *airplane2[i]);
  }

  f3d_lcd_fillScreen(WHITE);
  store_image("airplane.bmp", airplaneSize, airplaneSize);

  //assign space for enermy1
  enermy1 = malloc(airplaneSize * sizeof * enermy1);
  enermyWhite = malloc(airplaneSize * sizeof *enermyWhite);
  for(i = 0; i < airplaneSize; i++){
      enermy1[i] = malloc(airplaneSize * sizeof *enermy1[i]);
    enermyWhite[i] = malloc(airplaneSize * sizeof *enermyWhite[i]);
    }
  store_enermy1("enermy1.bmp", airplaneSize, airplaneSize);

  //assign space for enermy2
  enermy2 = malloc(enermy2Size * sizeof * enermy2);
  enermy2White = malloc(enermy2Size * sizeof *enermy2White);
  for(i = 0; i < enermy2Size; i++){
      enermy2[i] = malloc(enermy2Size * sizeof *enermy2[i]);
    enermy2White[i] = malloc(enermy2Size * sizeof *enermy2White[i]);
    }
  store_enermy2("enermy3.bmp", enermy2Size, enermy2Size);

    //assign space for bullet
  bullet = malloc(bulletSize * sizeof * bullet);
  bulletWhite = malloc(bulletSize * sizeof *bulletWhite);
  for(i = 0; i < bulletSize; i++){
    bullet[i] = malloc(bulletSize * sizeof *bullet[i]);
    bulletWhite[i] = malloc(bulletSize * sizeof *bulletWhite[i]);
    }
  store_bullet("bullet.bmp",bulletSize, bulletSize);
  

  //assign space for blood
  bloodImage = malloc(bloodSize * sizeof *bloodImage);
  bloodWhite = malloc(bloodSize * sizeof *bloodWhite);
  for(i = 0; i < bloodSize; i++){
    bloodImage[i] = malloc(bloodSize * sizeof *bloodImage[i]);
    bloodWhite[i] = malloc(bloodSize * sizeof *bloodWhite[i]);
    }
  store_heart("heart.bmp",bloodSize, bloodSize);




  enermy1_x = rand() % 89;
  enermy2_x = rand() %89;
  blood_x = rand() % 70;

  f3d_lcd_fillScreen(BLACK);
  draw_image(airplaneSize, airplaneSize, airplane, 30, 70);
  f3d_lcd_drawString(10, 120, "press Z to start", WHITE, BLACK);
  
  while(1){
    f3d_nunchuk_read(&nundata);
  
    if(nundata.z == 1){
      start = 1;
      f3d_lcd_fillScreen(WHITE);
    }

    if(start == 1){
    //draw score
    sprintf(scoreChar, "%i", score);
    f3d_lcd_drawString(100, 0, scoreChar, RED, WHITE);
    
    for(i = 0; i < blood; i++){
      drawBloodBar(bar_x, bar_y);
      bar_y += 8;
    }
    bar_y = 10; 
    
 
    
 

        //enermy2 fall down
     // draw_image(enermy2Size, enermy2Size, enermy2, 0, 120);
    if(enermy2_y == -40){
      enermy2_x = rand() % 89;
      enermy2_y = 130;
      // printf("!%d\n", enermy1_x);
    }
    if(enermy1Counter % 5 == 0){
      enermy2launch = 1;
    }
    
    if(enermy2launch){
      //if(blood_x < (enermy1_x - 20) && blood_x > (enermy1_x + 20)){
      draw_image(airplaneSize, airplaneSize, enermy2White, enermy2_x, enermy2_y);
      // printf("%d\n", enermy1_y);
      
      enermy2_y -= enermy2Speed;
      draw_image(airplaneSize, airplaneSize, enermy2, enermy2_x, enermy2_y);
      // 
    }


    
       //collsion
    // c = collsion(x, y, airplaneSize, enermy1_x, enermy1_y, airplaneSize);
    if(collsion(x, y, airplaneSize - 5, enermy2_x, enermy2_y, airplaneSize - 5)){
      draw_image(airplaneSize, airplaneSize, enermy2White, enermy2_x, enermy2_y);
      enermy2_x = rand() % 89;
      enermy2_y = 200;
      blood --;
      drawBloodBarWhite(2, blood * 8 + 10);
      enermy2launch = 0;

      bar_y = 10; 
      //printf("%d\n", blood);
      if(blood == 0){
	f3d_lcd_fillScreen(BLACK);
	f3d_lcd_drawString(10, 60, "Final Score:", WHITE, BLACK);
	f3d_lcd_drawString(80, 60, scoreChar, WHITE, BLACK);
	f3d_lcd_drawString(0, 80, "Please press reset button to start", WHITE, BLACK);
	break;
      }
    }

    for(i = 0; i < 5; i++){
      if (list[i].bullet_x != 0){ 
	
        if(collsion_bullet(list[i].bullet_x, list[i].bullet_y, bulletSize, enermy2_x, enermy2_y, airplaneSize)){
	  draw_image(airplaneSize, airplaneSize, enermyWhite, enermy2_x, enermy2_y);
	  enermy2_x = rand() % 89;
	  enermy2_y = 200;
	  draw_image(bulletSize, bulletSize, bulletWhite, list[i].bullet_x, list[i].bullet_y);
	  list[i].bullet_x = 0;
	  list[i].bullet_y = 0;
	  score += enermy2Point;
	}
	
      }
    }
    
    
    //enermy1 fall down
    if(enermy1_y == -40){
      enermy1_x = rand() % 89;
      enermy1_y = 130;
      enermy1Counter ++;
      // printf("!%d\n", enermy1_x);
    }
    draw_image(airplaneSize, airplaneSize, enermyWhite, enermy1_x, enermy1_y);
    // printf("%d\n", enermy1_y);
    
    enermy1_y -= enermy1Speed;
    draw_image(airplaneSize, airplaneSize, enermy1, enermy1_x, enermy1_y);



    //collsion
    // c = collsion(x, y, airplaneSize, enermy1_x, enermy1_y, airplaneSize);
    if(collsion(x, y, airplaneSize - 5, enermy1_x, enermy1_y, airplaneSize - 5)){
      draw_image(airplaneSize, airplaneSize, enermyWhite, enermy1_x, enermy1_y);
      enermy1_x = rand() % 89;
      enermy1_y = 200;
      blood --;
      drawBloodBarWhite(2, blood * 8 + 10);

      bar_y = 10; 
      enermy1Counter ++;
      //printf("%d\n", blood);
      if(blood == 0){
      f3d_lcd_fillScreen(BLACK);
      f3d_lcd_drawString(10, 60, "Final Score:", WHITE, BLACK);
      f3d_lcd_drawString(80, 60, scoreChar, WHITE, BLACK);
      f3d_lcd_drawString(0, 80, "Please press reset button to start", WHITE, BLACK);
      play_audio("gameover.wav");
      break;
      }
    }

     for(i = 0; i < 5; i++){
      if (list[i].bullet_x != 0){ 

        if(collsion_bullet(list[i].bullet_x, list[i].bullet_y, bulletSize, enermy1_x, enermy1_y, airplaneSize)){
	  draw_image(airplaneSize, airplaneSize, enermyWhite, enermy1_x, enermy1_y);
	  enermy1_x = rand() % 89;
	  enermy1_y = 200;
	  draw_image(bulletSize, bulletSize, bulletWhite, list[i].bullet_x, list[i].bullet_y);
	  list[i].bullet_x = 0;
	  list[i].bullet_y = 0;
	  score += enermy1Point;
	  enermy1Counter ++;
	}

      }
    }



    //blood
    if(blood_y == -40){
      blood_x = rand() % 70;
      blood_y = 200;
    
    }
    if(enermy1Counter % 10 == 0){
      bloodlaunch = 1;
    }
    
    if(bloodlaunch){
      //if(blood_x < (enermy1_x - 20) && blood_x > (enermy1_x + 20)){
      	draw_image(bloodSize, bloodSize, bloodWhite, blood_x, blood_y);
	blood_y -= bloodSpeed;
	draw_image(bloodSize, bloodSize, bloodImage, blood_x, blood_y);
	// 
    }
    if(collsion(x, y, airplaneSize - 5, blood_x, blood_y, bloodSize - 5)){
      draw_image(bloodSize, bloodSize, bloodWhite, blood_x, blood_y);
      blood_x = rand() % 70;
      blood_y = 200;
      blood ++;
      bloodlaunch = 0;
      //printf("%d\n", blood);
     }
    
    if(nundata.jx == 0){

      draw_image(airplaneSize, airplaneSize, airplane2, x, y);
      
      x -= airplaneSpeed;
      
      if(x < 0){
	x = 0;
      }
      draw_image(airplaneSize, airplaneSize, airplane, x, y);
      
      
    
    
    }else if(nundata.jx == 255){
    

      draw_image(airplaneSize, airplaneSize, airplane2,x, y);
      x += airplaneSpeed;
      if(x > border_x - airplaneSize){
	x = 128- airplaneSize;
      }
      draw_image(airplaneSize, airplaneSize, airplane, x, y);
  

  
  }

  if(nundata.jy == 0){


      draw_image(airplaneSize, airplaneSize, airplane2, x, y);
      y -= airplaneSpeed;
      if(y < 0){
	y =0;
      }
      draw_image(airplaneSize, airplaneSize, airplane, x, y);
    
  } else if(nundata.jy == 255){
    
    
    draw_image(airplaneSize, airplaneSize, airplane2,x,y);
    y += airplaneSpeed;
      if(y > border_y - airplaneSize){
	y = 160 - airplaneSize;
      }
      draw_image(airplaneSize, airplaneSize, airplane, x, y);
      
      
  }
  
  //launch bullet
  if(nundata.c == 1){
    while(nundata.c == 1){
      f3d_nunchuk_read(&nundata);
    }
    if(counter == 5){
      counter = 0;
    }
    list[counter].bullet_x = x + 15;
    list[counter].bullet_y = y + 40;
    launch = 1;
    counter ++;
    
  }
  if(launch == 1){
    for(i = 0; i < 5; i++){
      if (list[i].bullet_y > -10 && list[i].bullet_y < 160 && list[i].bullet_x != 0){ 
	//	printf("!%d\n", list[i].bullet_y);
		//bullet
	draw_image(bulletSize, bulletSize, bulletWhite, list[i].bullet_x, list[i].bullet_y);
	list[i].bullet_y += bulletSpeed;
	//bullet
	draw_image(bulletSize, bulletSize, bullet, list[i].bullet_x, list[i].bullet_y);
      }
    }
  }
  
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
