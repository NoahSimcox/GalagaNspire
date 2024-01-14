#include <os.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "libndls.h"
#include "Main.h"
#include <SDL/SDL_video.h>

SDL_Surface* screen;
SDL_Event event;
int numStars;
Uint32 map;

int main(void){

    Star* starList = setup();

    update(starList);

    free(starList);

	SDL_Quit();
	return 0;
}

Star* setup(){

    SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    Star* stars = drawStars();

    SDL_Delay(3000);

    drawGalagaText(nSDL_LoadImage(image_link), 18, 33);
    
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    SDL_Delay(3000);

    return stars;
}

Star* drawStars(){

    time_t t;
    srand((unsigned) time(&t));
    numStars = (rand() % 30) + 15;
    Star* starList = malloc(numStars * sizeof(Star));

    for (int i = 0; i < numStars; i++){

        int xPos = rand() % 320;
        int yPos = rand() % 240;
        int size = (rand() % 4) + 1;
        int starColor = (rand() % 76) + 180;

        map = SDL_MapRGB(screen->format, starColor, starColor, starColor);
        SDL_Rect rect = {xPos, yPos, size, size};
        Star star = {rect, map};
        starList[i] = star;
        SDL_FillRect(screen, &(star.rect), star.color);

    }

    return starList;
}

void update(Star* starList){

    while(true){

        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return;
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        
        for (int i = 0; i < numStars; i++){

            SDL_FillRect(screen, &(starList[i].rect), starList[i].color);
        }

        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
}


void drawGalagaText(SDL_Surface* sprite, int x, int y) {

    drawTile(sprite, x, y);


    // int numPositions;
    // Position* positions = galagaText(&numPositions);

    // for (int i = 0; i < numPositions; i++){

    //     int posX = positions[i].x;
    //     int posY = positions[i].y;

    //     SDL_Rect dest = {posX, posY, 1, 1};
    
	//     SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 255, 255, 255));
    // }
	
}

void drawTile(SDL_Surface *tileset, int x, int y) {

    SDL_Rect src_rect, screen_pos;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = TILE_WIDTH;
    src_rect.h = TILE_HEIGHT;
    screen_pos.x = x * TILE_WIDTH;
    screen_pos.y = y * TILE_HEIGHT;
    SDL_BlitSurface(tileset, &src_rect, screen, &screen_pos);
}