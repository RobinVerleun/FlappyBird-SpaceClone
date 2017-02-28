
#include "flappyClone.h"


int main(int argc, char **argv) 
{

	initialize();
	initialize_Mappy();
	draw_map();
	play_sounds();
	show_introduction();
	gameLoop();

	return 0;
}
END_OF_MAIN;

void gameLoop() 
{

	while(!gameover) {
		
		getInput();

		update_player();
		update_map();
		
		draw_map();		
		draw_player();
		draw_score();

		draw_screen();	

		if(gameover) check_continue();
	}
}

void initialize() 
{
	initialize_allegro();
	initialize_screen();
	initialize_player();
	initialize_music();
}

void initialize_allegro() 
{
	allegro_init();
	install_timer();
	install_keyboard();
}

void initialize_screen() 	
{
	set_color_depth(16);
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);

	buffer = create_bitmap(WIDTH, HEIGHT);
	clear(buffer);
}

void initialize_player()
{
	load_playerImage();
	setup_playerData();
}

void initialize_music() {
	if(install_sound(DIGI_AUTODETECT, MIDI_NONE,"") != 0) {
		allegro_message("Error initializing the sound system\n%s\n", allegro_error);
		return;
	}

	//load the wav files
	music = load_sample("./assets/hundouluo.wav");
	if(!music) {
		allegro_message("Error loading music file.");
		return;
	}
}

void play_sounds() {
	int panning = 128;
	int pitch = 1000;
	int volume = 128;
	music_isPlaying = 1;
	play_sample(music, volume, panning, pitch, TRUE);
}

void toggle_music() {
		
	if(music_isPlaying) {
		stop_sample(music);
		music_isPlaying = 0;
	} else {
		play_sounds();
	}
	clear_keybuf();
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

int initialize_Mappy()
{
	//Mappy Initializations
	if(MapLoad("./assets/gameMap.FMP") != 0) {
		allegro_message("Can't find the game map.");
		return 1;
	}
	mapxoff = 0;
	mapyoff = 0;
}

void update_map() 
{
	if(movescreen) {
		
		mapxoff = player->x - PLAYER_XSTART;
		if(mapxoff < 0) mapxoff = 0;
		if(mapxoff > (mapwidth * mapblockwidth - WIDTH)) {
			mapxoff = mapwidth * mapblockwidth - WIDTH;
		}
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



void update_player()
{

	if(collided(player->x, player->y)) {
		gameover = 1;
	}

	if( ++player->framecount > player->framedelay) {
		player->curframe = (player->curframe + 1) % 3;
		player->framecount = 0;
	}

	if(movescreen) {
		player->x += SCROLLSPEED;
		player->y += player->yspeed;
		player->yspeed += GRAVITY;
	}
}

void draw_player()
{
	draw_sprite(buffer, player_image[player->curframe],
		player->x-mapxoff, player->y);
}

void draw_score() {
	//clear_bitmap(buff);
	clear_keybuf();
	int low_h =  50;
	int high_h = 120;
	int low_w = WIDTH-400;
	int high_w = WIDTH-100;
	int FONT_COL = makecol(255, 255, 0);

	textprintf_right_ex(buffer, font, high_w, low_h, FONT_COL, -1,
	"Distance Traveled: %d", player->x - 80); 
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
	//Format data - bottom, top, and front of the player
	int bottom = y + player->height -10;
	int top = y+5;
	int front = x + player->width -10;
	int back = x+5;

	//Check to see if the bottom of the ship has hit top-left of tile
	if(collidedTL(front, bottom)) {
		return 1;
	} 

	//Check to see if the top of the ship has hit bottom-left of tile
	if(collidedBL(front, top)) {
		return 1;
	}

	//Check to see if the bottom of the ship has hit the top-right of tile
	if(collidedTR(back, bottom)) {
		return 1;
	}

	//Check to see if the top of the ship has hit the bottom-right of tile
	if(collidedBR(back, top)) {
		return 1;
	}
	
	//We're fine, return 0	
	return 0;	
}

int collidedTL(int x, int y)
{
    BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	//printf("%d %d %d %d\n", blockdata->tl, blockdata->tr, blockdata->bl, blockdata->br);
	return blockdata->tl;
}

int collidedBL(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->bl;
}

int collidedTR(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->tr;
}

int collidedBR(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->br;
}

void getInput() 
{
	//hit ESC to quit
    if (key[KEY_ESC])   gameover = 1;

/*

    //toggle the music
    if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && key[KEY_M]) {
    	toggle_music();
    	clear_keybuf();
    } 

   */
    if (key[KEY_H]) {
    	show_help();
    	clear_keybuf();
    }

        //toggle the music
    if (key[KEY_M]) {
    	toggle_music();
    	clear_keybuf();
    } 

    //Game Commands
    if (key[KEY_SPACE]) {
    	movescreen = 1;
    	jump();
    	clear_keybuf();
    }
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

void show_introduction()
{
	int low_h = HEIGHT/2 - 100;
	int high_h = HEIGHT/2 + 69;
	int low_w = WIDTH/2 - 300;
	int high_w = WIDTH/2 + 299;
	int FONT_COL = makecol(255,255,0);

	BITMAP *temp = create_bitmap(high_w - low_w, high_h - low_h);
	clear(temp);
	rectfill(temp, low_w, low_h, high_w,high_h, makecol(255,0,255));
	masked_blit(buffer, temp, 0, 0, 0, 0, high_w - low_w, high_h - low_h);
	destroy_bitmap(temp);

	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 5, FONT_COL, -1,
		"FLAPPY SHIP");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 20, FONT_COL, -1,
		"Navigate your way through without crashing!!");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 35, FONT_COL, -1,
		"Your ship has taken some damage- it won't be easy.");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 60, FONT_COL, -1,
		"Use SPACEBAR to boost the ship up.");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 75, FONT_COL, -1,
		"Press H for help in game.");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 135,  FONT_COL, -1, 
		"Make it to the end!");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 160,  FONT_COL, -1, 
		"Press ENTER to continue.");
	draw_screen();

	//wait for the user
	while(!key[KEY_ENTER]){};
	clear_keybuf();

}

void show_help() {
	clear_keybuf();
	int low_h = HEIGHT/2 - 100;
	int high_h = HEIGHT/2 + 69;
	int low_w = WIDTH/2 - 300;
	int high_w = WIDTH/2 + 299;
	int FONT_COL = makecol(255, 255, 0);

	BITMAP *temp = create_bitmap(high_w - low_w, high_h - low_h);
	clear(temp);
	rectfill(temp, low_w, low_h, high_w,high_h, makecol(255,0,255));
	masked_blit(buffer, temp, 0, 0, 0, 0, high_w - low_w, high_h - low_h);
	destroy_bitmap(temp);

	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 5, FONT_COL, -1,
		"FLAPPY SHIP");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 20, FONT_COL, -1,
		"Use SPACEBAR to boost the ship up.");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 35, FONT_COL, -1,
		"CTRL+H will show the help screen (this screen).");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 50, FONT_COL, -1,
		"CTRL+M will toggle the music.");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 75, FONT_COL, -1,
		"Make it all the way to the end of the maze!");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 150, FONT_COL, -1, 
		"Press ENTER to exit this screen.");
	draw_screen();

	//wait for the user
	while(!key[KEY_ENTER]){
		if(key[KEY_ESC]) {
			exit(1);
		}
	};

	//restore the screen
	draw_map();
	draw_player();
	draw_screen();
}

void check_continue() 
{
	clear_keybuf();
	int low_h = HEIGHT/2 - 100;
	int high_h = HEIGHT/2 + 69;
	int low_w = WIDTH/2 - 300;
	int high_w = WIDTH/2 + 299;
	int FONT_COL = makecol(255, 255, 0);

	BITMAP *temp = create_bitmap(high_w - low_w, high_h - low_h);
	clear(temp);
	rectfill(temp, low_w, low_h, high_w,high_h, makecol(255,0,255));
	masked_blit(buffer, temp, 0, 0, 0, 0, high_w - low_w, high_h - low_h);
	destroy_bitmap(temp);

	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 5, FONT_COL, -1,
		"You've crashed!");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 20, FONT_COL, -1,
		"Your score was:");
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 50, FONT_COL, -1,
		"%d", player->x - PLAYER_XSTART);
	textprintf_centre_ex(buffer, font, WIDTH / 2, low_h + 75, FONT_COL, -1,
		"Press Y to try again, or ESC to exit.");
	draw_screen();

	//wait for the user
	while(!key[KEY_Y]){
		if(key[KEY_ESC]) {
			exit(1);
		}
	}

	reset();

	//restore the screen
}

void reset()
{
	gameover = 0;
	movescreen = 0;

	mapxoff = 0;
	player->x = PLAYER_XSTART;
	player->y = PLAYER_YSTART;
}