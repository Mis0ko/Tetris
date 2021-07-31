#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL.h>
#include "surface.h"
#include <iostream>
using namespace std;
class WindSurface;

class Sprite : public Surface
{
    public :
    SDL_Rect srcBg = { 0,128, 96,128 }; 
    Sprite(const char* link_sprite);

};
#endif