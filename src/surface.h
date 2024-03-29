#ifndef _SURFACE_H_
#define _SURFACE_H_
using namespace std;
#include <SDL.h>
#include <string>
#include <iostream>

class Sprite;

class Surface
{
    protected:
    SDL_Surface* surf;

    public:
    Surface(void);
    SDL_Surface* get_surf();
    /***
     * set the surface with a link from a sprite
     * ***/
    void set_surf(const char* link_sprite);
    
    /***
     * set the surface with a SDL_Windows*
     * ***/
    void set_surf(SDL_Window* pWindow);
};

#endif