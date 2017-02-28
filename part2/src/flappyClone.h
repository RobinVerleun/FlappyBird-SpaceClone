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
	#define PLAYER_YSTART 682
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
		int music_isPlaying = 1;

		//Sounds
		SAMPLE *music;

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
	void gameLoop();

	void initialize_allegro();
	void initialize_screen();
	void initialize_player();
	void initialize_music();
	void play_sounds();
	void toggle_music();	
	void load_playerImage();
	void setup_playerData();
	int initialize_Mappy();
	void draw_score();

	void update_map();
	void update_player();

	void draw_map();
	void draw_player();
	void draw_show();
	void draw_screen();

	BITMAP* grabframe(BITMAP*, int, int, int, int, int, int);

	int collided(int, int);
	int collidedTL(int, int);
	int collidedBL(int, int);
	int collidedTR(int, int);
	int collidedBR(int, int);
	
	void getInput();
	void jump();

	void show_introduction();
	void show_help();
	void check_continue();
	void reset();




#endif
