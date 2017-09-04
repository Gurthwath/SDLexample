#pragma once
#include <SDL.h>
#include <iostream>
#include "Textures.h"

class Button {
private:
	int windowId = -1;
	SDL_Texture *texture = nullptr;
	int xPos = 0;
	int yPos = 0;
	int width = 0;
	int height = 0;
	void(*function)() = nullptr;
	Button *next = nullptr;
	Button *last = nullptr;

public:
	Button(int x, int y, const std::string &file, SDL_Renderer *ren) {
		setTexture(file, ren);
		xPos = x;
		yPos = y;
	}

	int getxPos() { return xPos; }
	int getyPos() { return yPos; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	void setWinId(int value) { windowId = value; }
	int getWinId() { return windowId; }
	void setFunction(void(*value)()) { function = value; }
	void (*getFunction(void))() { return function; }
	void setNext(Button *pointer) { next = pointer; }
	void setLast(Button *pointer) { last = pointer; }
	Button *getNext() { return next; }
	Button *getLast() { return last; }
	void setTexture(const std::string &file, SDL_Renderer *ren) {
		texture = loadTexture(file, ren);
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}

	void draw(int xOff, int yOff, SDL_Renderer *ren) {
		if (texture != nullptr) {
			renderTexture(texture, ren, xPos + xOff, yPos + yOff);
		}
	}
};

class ButtonList {
private:
	Button *head = nullptr;
	Button *tail = nullptr;
	Button *temp = nullptr;
	int length = 0;

public:
	int getLength() { return length; }
	Button *getHead() { return head; }
	Button *getTail() { return tail; }

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

	void add(int x, int y, const std::string &file, SDL_Renderer *ren) {
		temp = new Button(x, y, file, ren);
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

	void draw(int xOff, int yOff, SDL_Renderer *ren) {
		temp = head;
		while (temp != nullptr) {
			temp->draw(xOff, yOff, ren);
			temp = temp->getNext();
		}
	}

	Button *checkClicked(int x, int y) {
		temp = head;
		while (temp != nullptr) {
			if (x > temp->getxPos() && x < temp->getxPos() + temp->getWidth() && y > temp->getyPos() && y < temp->getyPos() + temp->getHeight()) {
				return temp;
			}
			temp = temp->getNext();
		}
		return nullptr;
	}
};

class Window {
private:
	int id = -1;
	std::string name = "";
	SDL_Texture *texture = nullptr;
	bool open = false;
	int xPos = 0;
	int yPos = 0;
	int width = 0;
	int height = 0;
	int topHeight = 20;
	bool moving = false;
	ButtonList buttons;
	Window *next = nullptr;
	Window *last = nullptr;

public:
	void setId(int value) { id = value; }
	int getId() { return id; }
	void setName(const std::string value) { name = value; }
	std::string getName() { return name; }
	void setNext(Window *value) { next = value; }
	Window *getNext() { return next; }
	void setLast(Window *value) { last = value; }
	Window *getLast() { return last; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	void setxPos(int value) { xPos = value; }
	int getxPos() { return xPos; }
	void setyPos(int value) { yPos = value; }
	int getyPos() { return yPos; }
	void setOpen(bool value) { open = value; }
	bool getOpen() { return open; }
	bool getMoving() { return moving; }
	void setMoving(bool value) { moving = value; }
	void toggleOpen() { open = !open; }

	void empty() { buttons.empty(); }

	Button *addButton(int x, int y, const std::string &file, SDL_Renderer *ren) {
		buttons.add(x, y, file, ren);
		return buttons.getTail();
	}
	void draw(SDL_Renderer *ren) {
		if (texture != nullptr && open) {
			renderTexture(texture, ren, xPos, yPos);
			buttons.draw(xPos, yPos, ren);
		}
	}
	void setTexture(const std::string &file, SDL_Renderer *ren) {
		texture = loadTexture(file, ren);
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}
	Button *clickedOn(int x, int y) {
		if (x - xPos < width && y - yPos < topHeight) {
			if (width - (x - xPos) < topHeight) { toggleOpen(); }
			else { moving = true; }
			return nullptr;
		}
		return buttons.checkClicked(x - xPos, y - yPos);
	}
};

class WindowList {
private:
	Window *head = nullptr;
	Window *tail = nullptr;
	Window *temp = nullptr;
	int length = 0;

public:
	Window *getHead() { return head; }
	void add(Window *window) {
		if (head == nullptr) {
			head = window;
			tail = window;
		}
		else if (tail == head) {
			head->setNext(window);
			tail = window;
			tail->setLast(head);
		}
		else {
			tail->setNext(window);
			window->setLast(tail);
			tail = window;
		}
		temp = nullptr;
		length++;
	}

	void empty() {
		while (head != nullptr) {
			temp = head->getNext();
			head->empty();
			delete head;
			head = temp;
		}
		head = nullptr;
		tail = nullptr;
		temp = nullptr;
		length = 0;
	}

	Window *get(int index) {
		temp = head;
		while (index != 0) {
			temp = temp->getNext();
			index--;
		}
		return temp;
	}

	Window *checkClick(int x, int y) {
		temp = head;
		while (temp != nullptr) {
			if (temp->getxPos() < x && temp->getxPos() + temp->getWidth() > x && temp->getyPos() < y && temp->getyPos() + temp->getHeight() > y && temp->getOpen()) {
				return temp;
			}
			temp = temp->getNext();
		}
		return nullptr;
	}

	void draw(SDL_Renderer *ren) {
		temp = head;
		while (temp != nullptr) {
			temp->draw(ren);
			temp = temp->getNext();
		}
	}
};