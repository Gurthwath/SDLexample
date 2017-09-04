#pragma once
#include <SDL.h>
#include <iostream>
#include "Textures.h"

class Block {
	SDL_Rect rect;
	SDL_Texture *texture = nullptr;
	int tiles;
	Block *next = nullptr;
	Block *last = nullptr;

public:
	Block(int x, int y, int w, SDL_Renderer *ren) {
		texture = loadTexture("world\\BlockTile.bmp", ren);
		rect.x = x;
		rect.y = y;
		tiles = (int)floor(w / 50);
		rect.w = tiles * 50;
		rect.h = 10;
	}

	void setNext(Block *pointer) { next = pointer; }
	void setLast(Block *pointer) { last = pointer; }
	Block *getNext() { return next; }
	Block *getLast() { return last; }
	SDL_Rect getRect() { return rect; }

	void draw(SDL_Renderer *ren, int pX, int pY, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
		for (int i = 0; i < tiles; i++) {
			renderTexture(texture, ren, (rect.x + i * 50) + (SCREEN_WIDTH / 2) - pX, rect.y + (SCREEN_HEIGHT / 2) - pY);
		}
	}
 };

class BlockList {
	Block *head = nullptr;
	Block *tail = nullptr;
	Block *temp = nullptr;
	int length = 0;
	int mapW = 1500;
	int mapH = 500;

public:
	int getLength() { return length; }
	int getMapW() { return mapW; }
	int getMapH() { return mapH; }
	void setMapW(int value) { mapW = value; }
	void setMapH(int value) { mapH = value; }
	Block *getHead() { return head; }

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

	void add(int x, int y, int w, SDL_Renderer *ren) {
		temp = new Block(x,y,w,ren);
		if (head == nullptr) {
			head = temp;
			tail = temp;
		} else if (tail == head) {
			head->setNext(temp);
			tail = temp;
			tail->setLast(head);
		} else {
			tail->setNext(temp);
			temp->setLast(tail);
			tail = temp;
		}
		temp = nullptr;
		length++;
	}

	Block *get(int index) {
		temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->getNext();
		}
		return temp;
	}

	void draw(SDL_Renderer *ren, int pX, int pY, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
		temp = head;
		while (temp != nullptr) {
			temp->draw(ren, pX, pY, SCREEN_WIDTH, SCREEN_HEIGHT);
			temp = temp->getNext();
		}
	}
};
