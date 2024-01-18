#include <os.h>
#include <SDL/SDL.h>
#include <SDL/SDL_config.h>
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
nSDL_Font* font;
int score = 0;

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
    font = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 188, 35, 35);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    Star* stars = drawStars();

    SDL_Surface* galagaText = nSDL_LoadImage(Galaga);
    drawSprite(galagaText, GALAGA_TEXT_X, GALAGA_TEXT_Y);
    
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

Meteor randMeteor(){

    time_t t;
    srand((unsigned) time(&t));

    Meteor meteor;

    int category = rand() % 10;
    if (category <= 4){
        meteor.type = normal;
        meteor.x = (rand() % 278) + 5;
        meteor.meteorSprite = nSDL_LoadImage(image_meteor_normal);
        meteor.width = 32;
        meteor.height = 32;
        meteor.health = 2;
    } else if (category <= 7){
        meteor.type = heavy;
        meteor.x = (rand() % 262) + 5;
        meteor.meteorSprite = nSDL_LoadImage(image_meteor_heavy);
        meteor.width = 48;
        meteor.height = 48;
        meteor.health = 4;
    } else{
        meteor.type = fast;
        meteor.x = (rand() % 296) + 5;
        meteor.meteorSprite = nSDL_LoadImage(image_meteor_fast);
        meteor.width = 14;
        meteor.height = 48;
        meteor.health = 1;
    }

    meteor.y = -1 * ((rand() % 20) + 42);
    meteor.isInitialized = true;
    
    return meteor;
}

void update(Star* starList){

    int doList[2];
    int lasersIncrement = 0;
    GameObject ship;
    ship.x = SHIP_X;
    ship.y = SHIP_Y;
    SDL_Rect lasers[10];
    int shootTimer = 0;
    Meteor meteors[10] = {{false}, {false}, {false}, {false}, {false}, {false}, {false}, {false}, {false}, {false}};
    int meteorIncrement = 0;
    int meteorTimer = 0;

    SDL_Surface* shipSprite = nSDL_LoadImage(image_ship);

    while(true){

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
                default:
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
                default:
                    break;
            }
        }

        for (int i = 0; i < 2; i++){

            switch(doList[i]){

                case empty:
                    break;
                case left:
                    if (ship.x > 5)
                        ship.x -= 3;
                    break;
                case right:
                    if (ship.x < 283)
                        ship.x += 3;
                    break;
                case shoot:
                    if (shootTimer <= 0){
                        lasers[lasersIncrement].x = ship.x + 15;
                        lasers[lasersIncrement].y = ship.y;
                        lasers[lasersIncrement].w = LASER_WIDTH;
                        lasers[lasersIncrement].h = LASER_LENGTH;
                        lasersIncrement++;
                        shootTimer = 25;
                    }
                    break;
            }
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        
        for (int j = 0; j < numStars; j++){

            SDL_FillRect(screen, &(starList[j].rect), starList[j].color);
        }

        if (meteorTimer <= 0){

            Meteor meteor = randMeteor();
            meteors[meteorIncrement] = meteor;
            meteorIncrement++;
            meteorTimer = levelUp();
        }

        collisionsCheck(meteors, lasers);

        for (int l = 0; l < 10; l++){

            if (!(meteors[l].isInitialized))
                continue;

            switch(meteors[l].type){

                case normal:
                    if (meteors[l].y >= 208){
                        meteors[l].x = TRASH_SPOT;
                        meteors[l].y = -400;
                        meteors[l].isInitialized = false;
                        score -= 2;
                        goto end;
                    }
                    break;
                case heavy:
                    if (meteors[l].y >= 192){
                        meteors[l].x = TRASH_SPOT;
                        meteors[l].y = -400;
                        meteors[l].isInitialized = false;
                        score -= 2;
                        goto end;
                    }
                    break;
                case fast:
                    if (meteors[l].y >= 192){
                        meteors[l].x = TRASH_SPOT;
                        meteors[l].y = -400;
                        meteors[l].isInitialized = false;
                        score -= 2;
                        goto end;
                    }
                    break;
            }
            
            int truncate = meteors[l].y;
            drawSprite(meteors[l].meteorSprite, meteors[l].x, truncate);

            if (meteors[l].type == normal)
                meteors[l].y += 0.75;
            else if (meteors[l].type == fast)
                meteors[l].y += 1.25;
            else
                meteors[l].y += 0.5;

            end:
        }

        for (int k = 0; k < 10; k++){

            if (lasers[k].y <= -9){
                
                lasers[k].x = TRASH_SPOT;
                continue;
            }

            if (lasers[k].w == 2){
                SDL_FillRect(screen, &(lasers[k]), SDL_MapRGB(screen->format, 242, 2, 2));
                lasers[k].y -= 2;
            }
        }

        drawSprite(shipSprite, ship.x, ship.y);

        if (lasersIncrement >= 10)
            lasersIncrement = 0;
        if (meteorIncrement >= 10)
            meteorIncrement = 0;

        shootTimer--;
        meteorTimer--;

        nSDL_DrawString(screen, font, 5, 5, "Score: %d", score);

        SDL_Flip(screen);   
    }

    SDL_FreeSurface(shipSprite);
    free(meteors);
}

void collisionsCheck(Meteor* meteors, SDL_Rect* lasers){

    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){

            if (!(meteors[i].isInitialized))
                goto endLoop;

            if (lasers[j].w != 2)
                continue;
            
            if (lasers[j].x >= meteors[i].x && lasers[j].x <= (meteors[i].width + meteors[i].x) && lasers[j].y <= (meteors[i].y + meteors[i].height) && lasers[j].y >= meteors[i].y){

                lasers[j].x = TRASH_SPOT;
                meteors[i].health--;

                if (meteors[i].health == 0){
                    meteors[i].x = TRASH_SPOT;
                    meteors[i].y = -400;
                    meteors[i].isInitialized = false;
                    score++;
                }
            }
        }

        endLoop:
    }
}

void animation(SDL_Surface* sprite, int x, int y, int anim){

    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(sprite->format, 255, 0, 255));
    SDL_Rect src_rect, screen_pos;
    src_rect.x = anim * 16;
    src_rect.y = 0;
    src_rect.w = 16;
    src_rect.h = 16;
    screen_pos.x = x;
    screen_pos.y = y;
    SDL_BlitSurface(sprite, &src_rect, screen, &screen_pos);
}

void drawSprite(SDL_Surface* sprite, int x, int y) {

    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(sprite->format, 255, 0, 255));
    SDL_Rect screen_pos;
    screen_pos.x = x;
    screen_pos.y = y;
    SDL_BlitSurface(sprite, NULL, screen, &screen_pos);
}

int levelUp(){

    if (score <= 10)
        return 200;
    else if (score <= 15)
        return 160;
    else if (score <= 20)
        return 140;
    else if (score <= 25)
        return 120;
    else if (score <= 30)
        return 100;
    else if (score <= 35)
        return 80;
    else
        return 60;
}