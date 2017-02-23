#ifndef _FLAPPYCLONE_H_
#define _FLAPPYCLONE_H_

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

	//Definitions
	#define MODE GFX_AUTODETECT_WINDOWED
	#define WIDTH 1000
	#define HEIGHT 992

	//Struct Definitions
	typedef struct SPRITE
	{
   	 	int dir, alive;
   	 	int x,y;
   	 	int width,height;
   	 	int xspeed,yspeed;
   	 	int xdelay,ydelay;
   	 	int xcount,ycount;
   	 	int curframe,maxframe,animdir;
   	 	int framecount,framedelay;
	}SPRITE;

	//Variables

		//Bitmaps
		BITMAP *buffer;

		//Mappy variables/offsets
		int mapxoff, mapyoff;

	//Function Prototypes
	void initialize();
	void move_screen();
	void draw_screen();

#endif
