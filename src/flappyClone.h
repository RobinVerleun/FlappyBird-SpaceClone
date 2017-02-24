#ifndef _FLAPPYCLONE_H_
#define _FLAPPYCLONE_H_

#include <stdio.h>
#include <allegro.h>
#include "mappyal.h"

	//Definitions
	#define MODE GFX_AUTODETECT_WINDOWED
	#define WIDTH 1000
	#define HEIGHT 992
	#define SCROLLSPEED 10
	
	#define PLAYER_XSTART 80
	#define PLAYER_YSTART 600
	#define PLAYER_JUMP -10

	#define GRAVITY 2

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

		//Game function variables
		int gameover = 0;
		int movescreen = 0;

		//Bitmaps
		BITMAP *buffer;
		BITMAP *player_image[3];

		//Sprites
		SPRITE *player;

		//Mappy variables/offsets
		int mapxoff, mapyoff;
		int oldx = 0;

	//Function Prototypes
	void initialize();
	void update_map();
	void draw_map();
	BITMAP* grabframe(BITMAP*, int, int, int, int, int, int);
	void initialize_player();
	void load_playerImage();
	void setup_playerData();
	void update_player();
	void draw_player();
	void draw_screen();
	int collided(int, int);
	void getInput();
	void jump();


#endif
