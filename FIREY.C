#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <mem.h>

#include "types.h"
#include "vga.h"
#include "pal.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

byte far *framebuf[4];

#define SETPIX(x,y,c) *(framebuf[(x) % 4]+((dword)SCREEN_WIDTH * (y) >> 2) + ((x) >> 2)) = c
#define GETPIX(x,y) *(framebuf[(x) % 4]+((dword)SCREEN_WIDTH * (y) >> 2) + ((x) >> 2))

void draw_fire(word x, word y, word w, word h)
{
  byte c;
  word o[9] = {0,0,0,0,0,0,0,0,0};
  word s[3] = {0,0,0};
  word i,j,dx;

  for(j = y; j < y + h; j++) {
    for(i = x; i < x + w; i++) {
      dx = (i % 3) * 3;
      o[dx    ]=GETPIX(i + 1, j  );
      o[dx + 1]=GETPIX(i + 1, j + 1);
      o[dx + 2]=GETPIX(i + 1, j + 2);
      s[i % 3] = o[dx + 0] + o[dx + 1] + o[dx + 2];
      c=(s[0]+s[1]+s[2]) / 9;
      c=MAX(c,2)-2;
      SETPIX(i,j,c);
    }
  }
}

void draw_fuel(word x, word y, word w, word h, byte c)
{
  dword i,j;
  for(i=x;i<x+w;i++) {
    c=(byte)(rand()&0xff > 0x7f ? 0xff : 0x00);
    SETPIX(i,y+h,c);
  }
}

int main()
{
  int i;
  word front, back;

  randomize();

  set_mode_y();
  front = vga_page[0];
  back = vga_page[1];

  set_palette( fire_pal );
  for( i = 0; i < 4; ++i ) {
    framebuf[i] = malloc( SCREEN_WIDTH*SCREEN_HEIGHT );
    if( framebuf[i] == NULL ) {
      printf("not enough memory\n");
      return 1;
    }
    memset( framebuf[i], 0x00, SCREEN_WIDTH*SCREEN_HEIGHT );
  }

  while(!kbhit())
  {
    draw_fuel( 0, 0, 319, 199, 255);
    draw_fire( 0, 150, 320, 50);
    blit2page( framebuf, back, 0, 0, 320, 200);
    wait_for_retrace();
    page_flip( &front, &back );
  }
  getch();
  set_text_mode();
  return 0;
}
