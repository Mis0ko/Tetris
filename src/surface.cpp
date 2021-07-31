#include "surface.h"

Surface::Surface()
{
    this->surf = NULL;
}

void Surface::set_surf(const char* link_sprite)
{
    this->surf = SDL_LoadBMP(link_sprite);
}
void Surface::set_surf(SDL_Window *pWindow)
{
    this->surf = SDL_GetWindowSurface(pWindow);
}

SDL_Surface* Surface::get_surf()
{
    return this->surf;
}