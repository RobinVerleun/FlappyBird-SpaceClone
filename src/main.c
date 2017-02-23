
#include "flappyClone.h"

int main(int argc, char **argv) {

	initialize();
	gameLoop();

	return 0;
}
END_OF_MAIN;

void gameLoop() {

	//Temp assignment for testing
	mapxoff = 0;
	mapyoff = 0;
	//End of temp

	while(!key[KEY_ESC]) {
		
		move_screen();
		draw_map();
		draw_screen();
		printf("Here\n");
	}
}

void initialize() {
	
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

	buffer = create_bitmap(WIDTH, HEIGHT);
	clear(buffer);
}

void move_screen() {
	mapxoff += 1;
		
	if(mapxoff > (mapwidth * mapblockwidth - WIDTH)) {
		mapxoff = mapwidth * mapblockwidth - WIDTH;
	}
}

void draw_map() {
	MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1);
	MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, WIDTH-1, HEIGHT-1, 0);
}

void draw_screen() {
	vsync();
	acquire_screen();
	blit(buffer, screen, 0, 0, 0, 0, WIDTH-1, HEIGHT-1);
	release_screen();
}