#pragma once
#include <Time.h>
#include <math.h>
#include <SDL.h>
#include <iostream>
#include "Textures.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Missile;
class MissileList;

void magicMissileFunction(int mouseX, int mouseY, Missile* missile);

class Missile {
private:
	int id = -1;
	int missType = -1;
	SDL_Rect *pos = new SDL_Rect();
	SDL_Rect *screenPos = new SDL_Rect();
	double angle = 80.0;
	SDL_Texture *spritesheet = nullptr;
	int frame = 0;
	int state = 0;
	time_t creation;
	time_t lastTick;
	double lifetime = 2; //in seconds
	void(*function)(int mouseX, int mouseY, Missile* missile) = nullptr;
	SDL_Rect *tempRect =  new SDL_Rect();
	SDL_Point *tempPoint = new SDL_Point();

	Missile *next = nullptr;
	Missile *last = nullptr;

public:

	int getxPos() { return pos->x; }
	void setxPos(int value) { pos->x = value; }
	int getyPos() { return pos->y; }
	void setyPos(int value) { pos->y = value; }
	double getAngle() { return angle; }
	void setAngle(double value) { angle = value; }
	int getWidth() { return pos->w; }
	void setWidth(int value) { pos->w = value; }
	int getHeight() { return pos->h; }
	void setHeight(int value) { pos->h = value; }
	int getType() { return missType; }
	void setType(int value) { missType = value; }
	void setTexture(const std::string file, SDL_Renderer *ren) { spritesheet = loadTexture(file, ren); }
	void setCreationTimer() { time(&creation); time(&lastTick); }
	double getLifetime() { return lifetime; }
	void setLifetime(double value) { lifetime = value; }

	SDL_Rect* getScreenPos() { return screenPos; }
	SDL_Rect* getPos() { return pos; }

	Missile* getNext() { return next; }
	void setNext(Missile* value) { next = value; }
	Missile* getLast() { return last; }
	void setLast(Missile* value) { last = value; }

	Missile(int type) {
		missType = type;
		setCreationTimer();
		setFunction();
	}
	Missile(int type, int iD) {
		missType = type;
		id = iD;
		setCreationTimer();
		setFunction();
	}

	void draw(SDL_Renderer *ren, int charxPos, int charyPos) {
		screenPos->x = pos->x - charxPos + SCREEN_WIDTH / 2 - pos->w / 2;
		screenPos->y = pos->y - charyPos + SCREEN_HEIGHT / 2 - pos->h / 2;
		screenPos->w = pos->w;
		screenPos->h = pos->h;
		tempRect->x = pos->w * frame;
		tempRect->y = 0;
		tempRect->w = pos->w;
		tempRect->h = pos->h;
		tempPoint->x = pos->w / 2;
		tempPoint->y = pos->h / 2;
		SDL_RenderCopyEx(ren, spritesheet, tempRect, screenPos, -angle * (180.0 / M_PI), tempPoint, SDL_FLIP_NONE);
		frame = (frame + 1) % 4;
	}

	void setFunction() {
		switch (missType) {
		case 0:
			function = magicMissileFunction;
			break;
		}
	}

	void callFunction(int mouseX, int mouseY) {
		function(mouseX, mouseY, this);
		lifetime -= difftime(time(NULL), lastTick);
		time(&lastTick);
	}
};

class MissileList {
private:
	Missile *head = nullptr;
	Missile *tail = nullptr;
	Missile *temp = nullptr;
	int length = 0;

public:
	int getLength() { return length; }
	Missile *getHead() { return head; }

	void empty() {
		while (head != nullptr) {
			temp = head->getNext();
			delete head;
			head = temp;
		}
		head = nullptr;
		tail = nullptr;
		temp = nullptr;
		length = 0;
	}

	void add(Missile* toAdd) {
		temp = toAdd;
		if (head == nullptr) {
			head = temp;
			tail = temp;
		}
		else if (tail == head) {
			head->setNext(temp);
			tail = temp;
			tail->setLast(head);
		}
		else {
			tail->setNext(temp);
			temp->setLast(tail);
			tail = temp;
		}
		temp = nullptr;
		length++;
	}

	void remove(Missile *toRem) {
		temp = toRem;
		if (temp->getLast() != nullptr) {
			temp->getLast()->setNext(temp->getNext());
		} else {
			head = temp->getNext();
		}
		if (temp->getNext() != nullptr) {
			temp->getNext()->setLast(temp->getLast());
		} else {
			tail = temp->getLast();
		}
		delete temp;
		temp = nullptr;
		length--;
	}
};

void magicMissileFunction(int mouseX, int mouseY, Missile* missile) {
	missile->setAngle(atan2(missile->getScreenPos()->x - mouseX, missile->getScreenPos()->y - mouseY));
	missile->getPos()->x -= (int)(SDL_sin(missile->getAngle()) * 10.0);
	missile->getPos()->y -= (int)(SDL_cos(missile->getAngle()) * 10.0);
	if (abs(missile->getScreenPos()->x - mouseX) < 10 && abs(missile->getScreenPos()->y - mouseY) < 10) {
		missile->setLifetime(0);
	}
}
