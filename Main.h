#include <os.h>
#include <SDL/SDL_config.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "libndls.h"
#include <SDL/SDL_video.h>

 
 typedef struct{
        SDL_Rect rect;
        Uint32 color;
    } Star;

Star* setup();
Star* drawStars();
SDL_Surface* galagaTextureLoad(char *filename);