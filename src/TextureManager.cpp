#include "TextureManager.h"

SDL_Texture* loadTexture(const char* file_name, SDL_Renderer* renderer)
{
	SDL_Surface* temp_surface = IMG_Load(file_name);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	return texture;
}

void draw(SDL_Texture* tex, SDL_Renderer* renderer, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(renderer, tex, &src, &dest);
}
