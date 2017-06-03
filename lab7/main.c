/* main.c ---
 *
 * Filename: main.c
 * Description:
 * Author: Qingyue Long (qinglong) and Maggie Oates (moates)
 * Maintainer:
 * Created: Thu Jan 10 11:23:43 2013
 /* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
 
  // Set up your inits before you go ahead
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_user_btn_init();

  int arrow_length = 30;
  uint16_t background = WHITE;

  float mag_data[3];
  float accel_data[3];

  //in degrees
  float pitch;
  float roll;

  //in radians
  float heading;

  float xhead, yhead;
  int show_tilt = 1;

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
  
  f3d_lcd_fillScreen2(background);
  
  while(1){
    f3d_mag_read(mag_data);
    f3d_accel_read(accel_data);
    a_x=accel_data[0];
    a_y=accel_data[1];
    a_z=accel_data[2];
    m_x=mag_data[0];
    m_y=mag_data[1];
    m_z=mag_data[2];  
    pitch = atanf(a_x/sqrt(pow(a_y, 2) + pow(a_z,2)));
    roll = atanf(a_y/sqrt(pow(a_x, 2) + pow(a_z,2))); 

    if(user_btn_read()){
      show_tilt  = show_tilt ? 0 : 1;	
      f3d_lcd_fillScreen2(background);
      while(user_btn_read());
    }
    else{
    if(show_tilt){
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
	delay(100); 


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
	else{
	//Compass
   

    
    xhead=m_x * cos(pitch)+ m_z * sin(pitch);
    yhead=m_x * sin(roll) * sin(pitch) + 
          m_y * cos(roll) - 
          m_z * sin(roll) * cos(pitch); 

    heading = atan2f(yhead, xhead)-M_PI/4;
    

    x = arrow_length*cos(heading) + (ST7735_width / 2.0);
    y = arrow_length*sin(-1*heading) + (ST7735_height / 2.0);

    // printf("X: %d, Y: %d\n", x, y);
    f3d_lcd_drawChar(x, y, 'O', BLUE, background);

 
    delay(50);
   f3d_lcd_drawChar(x, y, 'O', background, background);
    
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
