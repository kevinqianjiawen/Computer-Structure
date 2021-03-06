/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Maggie Oates (moates) and Qingyue Long(qinglong) 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 10/20/2016
 *           By: 
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <stdio.h>
#include <math.h>
//Constants
uint16_t background = WHITE;


void draw_compass(float * mag_data, float * accel_data){
  int arrow_length = 30;
  
  //in degrees
  float pitch;
  float roll;
  float heading;

  float xhead, yhead;
  //positions on the LCD
  int x,y;
  
  float a_x, a_y, a_z;
  float m_x, m_y, m_z;
 
  
    a_x=accel_data[0];
    a_y=accel_data[1];
    a_z=accel_data[2];
    m_x=mag_data[0];
    m_y=mag_data[1];
    m_z=mag_data[2];  
    pitch = atanf(a_x/sqrt(pow(a_y, 2) + pow(a_z,2)));
    roll = atanf(a_y/sqrt(pow(a_x, 2) + pow(a_z,2))); 


    xhead=m_x * cos(pitch)+ m_z * sin(pitch);
    yhead=m_x * sin(roll) * sin(pitch) + 
          m_y * cos(roll) - 
          m_z * sin(roll) * cos(pitch);     
    m_y * cos(roll) - 
    m_z * sin(roll) * cos(pitch); 

    heading = atan2f(yhead, xhead)-M_PI/4;
    

    x = arrow_length*cos(heading) + (ST7735_width / 2.0);
    y = arrow_length*sin(-1*heading) + (ST7735_height / 2.0);

    // printf("X: %d, Y: %d\n", x, y);
    f3d_lcd_drawChar(x, y, 'O', BLUE, background);

 
    delay(20);
    f3d_lcd_drawChar(x, y, 'O', background, background);  
	       	
}

void draw_tilt(float * mag_data, float * accel_data){

  //in radians
  float pitch;
  float roll;

  //positions on the LCD
  int x,y;
  
  float a_x, a_y, a_z;
  float m_x, m_y, m_z;
  int bar_width = 10;

  uint16_t pitch_bar[bar_width];
  uint16_t erase[bar_width];
  for (y = 0; y < bar_width + 1; y++){
          pitch_bar[y] = BLUE;
	  erase[y] = background;
   }
  
    a_x=accel_data[0];
    a_y=accel_data[1];
    a_z=accel_data[2];
    m_x=mag_data[0];
    m_y=mag_data[1];
    m_z=mag_data[2];  
    pitch = atanf(a_x/sqrt(pow(a_y, 2) + pow(a_z,2)));
    roll = atanf(a_y/sqrt(pow(a_x, 2) + pow(a_z,2))); 

      //Tilt
      int scaled_pitch =fabs( pitch / M_PI * (ST7735_height));
      int scaled_roll =  fabs(roll / M_PI * (ST7735_width));
      int pitch_sign = pitch > 0;
      int roll_sign = roll > 0;

	if(pitch_sign){
	    f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2-scaled_pitch+1, ST7735_width/2+5,
				   ST7735_height/2,MADCTLGRAPHICS);
	}
	else{
	  f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2+1, ST7735_width/2+5,
				   ST7735_height/2 - scaled_pitch,MADCTLGRAPHICS);
	}

        for (y=0;y<scaled_pitch; y++) {
	  f3d_lcd_pushColor(pitch_bar, bar_width);
         } 

  
	uint16_t roll_bar[scaled_roll];
	uint16_t roll_erase[scaled_roll];
	for(x = 0; x < scaled_roll; x++){
	  roll_bar[x] = GREEN;
	  roll_erase[x] = background;
	}

	if(roll_sign){
	   f3d_lcd_setAddrWindow (ST7735_width/2-scaled_roll+1, ST7735_height/2-4, ST7735_width/2,
				   ST7735_height/2+5,MADCTLGRAPHICS);
	}
	else{
	  f3d_lcd_setAddrWindow (ST7735_width/2+1, ST7735_height/2-4, ST7735_width/2+scaled_roll,
				   ST7735_height/2 + 5,MADCTLGRAPHICS);
	}

        for (y=0;y<bar_width; y++) {
	  f3d_lcd_pushColor(roll_bar, scaled_roll);
         } 
	delay(30); 


	//Erasing bars
     	for (y=0;y<bar_width; y++) {
	  f3d_lcd_pushColor(roll_erase, scaled_roll);
	 }
	
	if(pitch_sign){
	    f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2-scaled_pitch+1, ST7735_width/2+5,
				   ST7735_height/2,MADCTLGRAPHICS);
	}
	else{
	  f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2+1, ST7735_width/2+5,
				   ST7735_height/2 - scaled_pitch,MADCTLGRAPHICS);
	}

     	for (y=0;y<scaled_pitch; y++) {
	  f3d_lcd_pushColor(erase, bar_width);
	 }
       
}

void draw_gyro(float* f){
  char array0[50];
  char array1[50];
  char array2[50];
	// Convert to character
        sprintf(array0, "%f", f[0]);
	sprintf(array1, "%f", f[1]);
	sprintf(array2, "%f", f[2]);

	// Draws it...
  	f3d_lcd_drawString(10, 10, array0, RED, background);
	f3d_lcd_drawString(10, 30, array1, GREEN, background);
	f3d_lcd_drawString(10, 50, array2, BLUE, background);
	
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
	 delay(300);
        uint16_t renew[ST7735_width];
        for (y = 0; y < ST7735_width; y++){
          renew[y] = WHITE;
        f3d_lcd_setAddrWindow (0,80,ST7735_width-1,160,MADCTLGRAPHICS);}
        for (y=0;y<61; y++) {
          f3d_lcd_pushColor(renew,ST7735_width);
	}}

void draw_nunchuk_tilt(short x, short y, short z){
  int bar_width = 10;
  x = x - 512;
  y = y - 512;

  float pitch = atanf(x / sqrt(pow(y, 2) + pow(z, 2)));
  float roll = atanf(y / sqrt(pow(x, 2) + pow(z, 2)));

  uint16_t pitch_bar[bar_width];
  uint16_t erase[bar_width];
  for (y = 0; y < bar_width + 1; y++){
          pitch_bar[y] = BLUE;
	  erase[y] = background;
   }

  

      //Tilt
      int scaled_pitch =fabs( pitch / M_PI * (ST7735_height));
      int scaled_roll =  fabs(roll / M_PI * (ST7735_width));
      int pitch_sign = pitch > 0;
      int roll_sign = roll > 0;

	if(pitch_sign){
	    f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2-scaled_pitch+1, ST7735_width/2+5,
				   ST7735_height/2,MADCTLGRAPHICS);
	}
	else{
	  f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2+1, ST7735_width/2+5,
				   ST7735_height/2 - scaled_pitch,MADCTLGRAPHICS);
	}

        for (y=0;y<scaled_pitch; y++) {
	  f3d_lcd_pushColor(pitch_bar, bar_width);
         } 

  
	uint16_t roll_bar[scaled_roll];
	uint16_t roll_erase[scaled_roll];
	for(x = 0; x < scaled_roll; x++){
	  roll_bar[x] = GREEN;
	  roll_erase[x] = background;
	}

	if(roll_sign){
	   f3d_lcd_setAddrWindow (ST7735_width/2-scaled_roll+1, ST7735_height/2-4, ST7735_width/2,
				   ST7735_height/2+5,MADCTLGRAPHICS);
	}
	else{
	  f3d_lcd_setAddrWindow (ST7735_width/2+1, ST7735_height/2-4, ST7735_width/2+scaled_roll,
				   ST7735_height/2 + 5,MADCTLGRAPHICS);
	}

        for (y=0;y<bar_width; y++) {
	  f3d_lcd_pushColor(roll_bar, scaled_roll);
         } 
	//delay(100); 


	//Erasing bars
     	for (y=0;y<bar_width; y++) {
	  f3d_lcd_pushColor(roll_erase, scaled_roll);
	 }
	
	if(pitch_sign){
	    f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2-scaled_pitch+1, ST7735_width/2+5,
				   ST7735_height/2,MADCTLGRAPHICS);
	}
	else{
	  f3d_lcd_setAddrWindow (ST7735_width/2-4, ST7735_height/2+1, ST7735_width/2+5,
				   ST7735_height/2 - scaled_pitch,MADCTLGRAPHICS);
	}

     	for (y=0;y<scaled_pitch; y++) {
	  f3d_lcd_pushColor(erase, bar_width);
	 } 

}

int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  f3d_gyro_init();
        delay(10);
  f3d_uart_init();
	delay(10);	
  f3d_lcd_init();
	delay(10);
  f3d_delay_init();
	delay(10);
	f3d_i2c1_init();
  delay(10);
  f3d_nunchuk_init();
	delay(10);
	f3d_accel_init();
	delay(10);
	f3d_mag_init();

	nunchuk_t data;
	int screen = 0; //between 0 and 3
	int num_screens = 4;

        f3d_lcd_fillScreen2(background);

	while(1){
		f3d_nunchuk_read(&data);
		if(data.z == 1){
		  screen++;
		  while(data.z ==1){
		    f3d_nunchuk_read(&data);
		  }

		  screen %= num_screens;
		  f3d_lcd_fillScreen2(background);
		}
                if(data.c == 1){
                  screen = screen +3;
		  while(data.c ==1){
		    f3d_nunchuk_read(&data);
		  }
                  screen %= num_screens;
                  f3d_lcd_fillScreen2(background);
		}

		if(screen == 0){
		  // f3d_gyro_init();
		  delay(10);		  
		  float g_data[3];
	          f3d_gyro_getdata(g_data);
		  draw_gyro(g_data);
		 
		  f3d_lcd_drawString(10, ST7735_height-20, "the graph of gyro", RED, background);
		}
	
		else if(screen == 1){
		  float mag_data[3];
		  float accel_data[3];

		  f3d_mag_read(mag_data);
		  f3d_accel_read(accel_data);
		  draw_compass(mag_data, accel_data);
		  f3d_lcd_drawString(10, ST7735_height-20, "compass", RED, background);
		}
		else if(screen == 2){
		  float mag_data[3];
		  float accel_data[3];

		  f3d_mag_read(mag_data);
		  f3d_accel_read(accel_data);
		  draw_tilt(mag_data, accel_data);
		  f3d_lcd_drawString(10, ST7735_height-20, "tilt angle", RED, background);
		}
		else{
		  draw_nunchuk_tilt(data.ax, data.ay, data.az);		  
		  f3d_lcd_drawString(10, ST7735_height-20, "nunchuk tilt", RED, background);
		}
		
		printf("%d %d %d %d %d %d %d\n", data.jx, data.jy, data.ax, data.ay, data.az, data.c, data.z);
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
