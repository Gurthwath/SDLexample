#pragma once
#include <SDL.h>
#include <iostream>
#include "Textures.h"
#include "BlockList.h"

class Player {
private:
	int xPos = 0;
	int yPos = 0;
	int xVel = 0;
	float yVel = 0;
	int width = 0;
	int height = 0;
	int speed = 6;
	float grav = -1;
	int maxJumps = 2;
	int jumps = maxJumps;
	SDL_Texture* texture = nullptr;
	Block *temp = nullptr;
	SDL_Rect tempRect;

public:
	void setxVel(int value) { xVel = value; }
	void setyVel(float value) { yVel = value; }
	int getxPos() { return xPos; }
	int getyPos() { return yPos; }
	int getxVel() { return xVel; }
	float getyVel() { return yVel; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	void setJumps(int value) { jumps = value; }
	int getJumps() { return jumps; }
	void setTexture(const std::string &file, SDL_Renderer *ren) {
		texture = loadTexture(file, ren);
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}

	void move(int mapW, int mapH, BlockList level) {
		xPos += xVel * speed;
		yPos += yVel * speed;
		if (xPos < width / 2) { xPos = width / 2; } //collision checking with the edge
		if (xPos > mapW - width / 2) { xPos = mapW - width / 2; }
		if (yPos < width / 2) { yPos = height / 2; }
		if (yPos > mapH - height / 2) { xPos = 0; yPos = 0; } //respawn if you fall off the bottom

		temp = level.getHead();
		while (temp != nullptr) { //collision checking with platforms
			tempRect = temp->getRect();
			if (xPos > tempRect.x - (width / 2) && xPos < tempRect.x + tempRect.w + (width / 2)) { //lateral bounds
				if (yPos + height / 2 < tempRect.y) { //over the platform
					temp = temp->getNext();
					continue;
				} else if (yPos - yVel * speed + height / 2 <= tempRect.y && yVel > 0) { //landing
					jumps = maxJumps;
					yVel = 0;
					yPos = tempRect.y - height / 2;
				} else if (yPos + height / 2 > tempRect.y && yPos - height / 2 < tempRect.y && yVel >= 0) { //allowing stairs
					jumps = maxJumps;
					yVel = 0;
					yPos = tempRect.y - height / 2;
				}
			}
			temp = temp->getNext();
		}
	}

	void addGravity(const float GRAVITY) {
		if (grav < 0) {
			yVel += GRAVITY;
		} else {
			yVel += grav;
		}
	}
	void setGrav(float value) {
		grav = value;
	}

	void draw(SDL_Renderer *ren, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
		if (texture != nullptr) {
			renderTexture(texture, ren, (SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
		}
	}
};
