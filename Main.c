#include <os.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "libndls.h"
#include "Main.h"

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

    SDL_Surface* galagaText = nSDL_LoadImage(Galaga);
    drawGalagaText(galagaText, GALAGA_TEXT_X, GALAGA_TEXT_Y);
    
    SDL_Flip(screen);
    SDL_FreeSurface(galagaText);

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

    int doList[2];
    int increment;
    GameObject ship;
    ship.x = SHIP_X;
    ship.y = SHIP_Y;

    while(true){

        SDL_Surface* shipSprite = nSDL_LoadImage(image_ship);

        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN){

            switch (event.key.keysym.sym){

                case SDLK_ESCAPE:
                    return;
                case SDLK_LEFT:
                    doList[0] = left;
                    break;
                case SDLK_RIGHT:
                    doList[0] = right;
                    break;
                case SDLK_BACKSPACE:
                    doList[1] = shoot;
                    break;
            }
        }

        if (event.type == SDL_KEYUP){

            switch (event.key.keysym.sym){

                case SDLK_LEFT:
                    doList[0] = empty;
                    break;
                case SDLK_RIGHT:
                    doList[0] = empty;
                    break;
                case SDLK_BACKSPACE:
                    doList[1] = empty;
                    break;
            }
        }

        for (int i = 0; i < 2; i++){

            switch(doList[i]){

                case empty:
                    break;
                case left:
                    if (ship.x > 5)
                        ship.x -= 2;
                    break;
                case right:
                    if (ship.x < 283)
                        ship.x += 2;
                    break;
                case shoot:
                    ship.y--;
                    break;
            }
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        
        for (int i = 0; i < numStars; i++){

            SDL_FillRect(screen, &(starList[i].rect), starList[i].color);
        }

        drawGameObject(shipSprite, ship.x, ship.y);

        SDL_Flip(screen);
        SDL_FreeSurface(shipSprite);
    }
}


void drawGalagaText(SDL_Surface* sprite, int x, int y) {

    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(sprite->format, 255, 0, 255));

    drawSprite(sprite, x, y);
	
}

void drawGameObject(SDL_Surface* image, int x, int y){

    SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 255));

    drawSprite(image, x, y);
}

void drawSprite(SDL_Surface* sprite, int x, int y) {

    SDL_Rect screen_pos;
    screen_pos.x = x;
    screen_pos.y = y;
    SDL_BlitSurface(sprite, NULL, screen, &screen_pos);
}