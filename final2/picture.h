#include<stdio.h>

typedef struct {
  unsigned char magic [2];
} bmpfile_magic;

typedef struct {
  uint32_t filesz ;
  uint16_t creator1 ;
  uint16_t creator2 ;
  uint32_t bmp_offset ;
} bmpfile_header;

typedef struct {
  uint32_t header_sz ;
  int32_t width ;
  int32_t height ;
  uint16_t nplanes ;
  uint16_t bitspp ;
  uint32_t compress_type ;
  uint32_t bmp_bytesz ;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors ;
  uint32_t nimpcolors ;
} BITMAPINFOHEADER ;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} bmp;

uint16_t convert_pixcel(bmp pixel);
void draw_image(int width, int height, uint16_t** a, int x, int y);
void die (FRESULT rc);
