#pragma once
#include <SDL.h>
#include <iostream>

const std::string fileHeader = SDL_GetBasePath();

void logSDLError() {
	std::cout << " error: " << SDL_GetError() << std::endl;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren) {
	std::string filePath = fileHeader + "Assets\\" + file;
	SDL_Texture* texture = nullptr;
	SDL_Surface *loadedImage = SDL_LoadBMP(filePath.c_str());
	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		if (texture == nullptr) {
			logSDLError();
		}
	}
	else {
		logSDLError();
	}
	return texture;
}
