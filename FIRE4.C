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

byte far *framebuf;

#define SETPIX(x,y,c) *(framebuf + (dword)SCREEN_WIDTH * (y) + (x)) = c
#define GETPIX(x,y) *(framebuf + (dword)SCREEN_WIDTH * (y) + (x))

void draw_fire(word x, word y, word w, word h)
{
  byte c;
  word s[3]={0,0,0};
  word i,j;

  for(j = y; j < y + h; j++){
    for(i = x; i < x + w; i++) {
      s[i % 3] = GETPIX(i + 1, j) + GETPIX(i + 1, j + 1) + GETPIX(i + 1, j + 2);
      c = (s[0] + s[1] + s[2]) / 9;
      c = MAX(c,2) - 2;
      SETPIX(i,j,c);
    }
  }
}

void draw_fuel(word x, word y, word w, word h, byte c)
{
  dword i;
  byte f;
  for(i = x; i < x + w; i++) {
    f = (byte)(rand() & 0xff > 0x7f ? c : 0);
    SETPIX(i,y + h,f);
  }
}

int main()
{
  byte fuel = 255, df = 0;
  int i;
  randomize();
  set_mode_y();
  set_palette(fire_pal);
  framebuf = malloc(SCREEN_WIDTH*SCREEN_HEIGHT);
  if(framebuf == NULL) {
    printf("not enough memory\n");
    return 1;
  }
  memset(framebuf, 0x00, SCREEN_WIDTH*SCREEN_HEIGHT);
  while( fuel > df ) {
    if(kbhit()){
      getch();
      df = 5;
    }
    fuel -= df;
    draw_fuel(0,0,80,52,fuel);
    draw_fire(0,0,80,52);
    wait_for_retrace();
    blit4(framebuf,0,0,80,50);
  }
  set_text_mode();
  return 0;
}

