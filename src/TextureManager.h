#pragma once

#include <SDL.h>
#include <SDL_image.h>

SDL_Texture* loadTexture(const char* file_name, SDL_Renderer* renderer);
void draw(SDL_Texture* tex, SDL_Renderer* renderer, SDL_Rect src, SDL_Rect dest);
