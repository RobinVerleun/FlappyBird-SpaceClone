
#include "flappyClone.h"


int main(int argc, char **argv) 
{

	initialize();
	gameLoop();

	return 0;
}
END_OF_MAIN;

void gameLoop() 
{

	//Temp assignment for testing
	
	//End of temp

	while(!gameover) {
		
		getInput();

		update_map();
		update_player();

		draw_map();		
		draw_player();

		draw_screen();
		
		rest(20);
	}
}

void initialize() 
{
	// Allegro Initializations
	allegro_init();
	install_timer();
	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);

	//Mappy Initializations
	if(MapLoad("./assets/gameMap.FMP") != 0) {
		allegro_message("Can't find the game map.");
		return 1;
	}
	mapxoff = 0;
	mapyoff = 0;

	//Buffer Initialization
	buffer = create_bitmap(WIDTH, HEIGHT);
	clear(buffer);

	//Player Initialization
	initialize_player();
}


void update_map() 
{
	if(movescreen) mapxoff += SCROLLSPEED;

	if(mapxoff > (mapwidth * mapblockwidth - WIDTH)) {
		mapxoff = mapwidth * mapblockwidth - WIDTH;
	}
}

void draw_map() 
{
	MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1);
	MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1, 0);
}

BITMAP *grabframe(BITMAP *source,
				int width, int height,
				int startx, int starty,
				int columns, int frame)
{
	BITMAP *temp = create_bitmap(width,height);
    int x = startx + (frame % columns) * width;
    int y = starty + (frame / columns) * height;
    blit(source,temp,x,y,0,0,width,height);
    return temp;
}

void initialize_player()
{
	load_playerImage();
	setup_playerData();
}


void load_playerImage() 
{
	BITMAP *temp;
	int n;
	temp = load_bitmap("./assets/spaceship.bmp", NULL);
	for(n = 0; n < 3; n++) {
		player_image[n] = grabframe(temp, 144, 80, 0, 0, 1, n);
	}
	destroy_bitmap(temp);
}

void setup_playerData() 
{
	player = malloc(sizeof(SPRITE));
	player->x = PLAYER_XSTART;
	player->y = PLAYER_YSTART;
	player->curframe = 0;
	player->framecount = 0;
    player->framedelay = 6;
    player->maxframe = 2;
    player->width = player_image[0]->w;
    player->height = player_image[0]->h;
}

void update_player()
{

	collided(player->x + player->width, player->y + player->height);

	if( ++player->framecount > player->framedelay) {
		player->curframe = (player->curframe + 1) % 3;
		player->framecount = 0;
	}

	if(movescreen) {
		player->y += player->yspeed;
		player->yspeed += GRAVITY;
	}
}

void draw_player()
{
	draw_sprite(buffer, player_image[player->curframe],
		player->x, player->y);
}

void draw_screen() 
{
	vsync();
	acquire_screen();
	blit(buffer, screen, 0, 0, 0, 0, WIDTH-1, HEIGHT-1);
	release_screen();
}

int collided(int x, int y)
{
    BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	printf("%d %d %d %d\n", blockdata->tl, blockdata->tr, blockdata->bl, blockdata->br);
	return blockdata->tl;
}

int collidedFront(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->bl;
}

void getInput() 
{
	//hit ESC to quit
    if (key[KEY_ESC])   gameover = 1;

/*
    //check for flag commands
    if (key[KEY_LCONTROL] && key[KEY_C]) {
    	can_collide = (can_collide + 1) % 2;
    	clear_keybuf();
    } 

    //toggle the music
    if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && key[KEY_M]) {
    	toggle_music();
    	clear_keybuf();
    } 

    //toggle the music
    if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && key[KEY_H]) {
    	show_help();
    	clear_keybuf();
    }
    
    if (key[KEY_P]) {
    	paused();
    }
   */

    //WASD or Arrows
    if (key[KEY_SPACE]) {
    	movescreen = 1;
    	jump();
    	clear_keybuf();
    }

   // rest(20);
}

void jump()
{
	player->yspeed = PLAYER_JUMP;
}

void cleanup() 
{
	int n;
	MapFreeMem();
	destroy_bitmap(buffer);
	for(n = 0; n < 3; n++) {
		destroy_bitmap(player_image[n]);
	}
	free(player);
	allegro_exit();
}