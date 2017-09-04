#include <iostream>
#include <SDL.h>
#include <time.h>
#include "Player.h"
#include "Textures.h"
#include "BlockList.h"
#include "Window.h"
#include "Keybinds.h"
#include "MissileList.h"
#include "windowInit.h"

/*  
 !  0.0.4a todo:
 !  spells - missile framework finished, spells not started
 !  cycles - not started
 */

void exitGame();
void toggleOptionsWin();
void setKeybindLeft();
void setKeybindRight();
void setKeybindJump();
void setKeybindSpellOne();
void setKeybindSpellTwo();
void setKeybindSpellThree();
void setKeybindSpellFour();
void setKeybindMainMenu();
void setKeybindInventory();

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;
const int color1[3] = { 55, 14, 8 };
const int color2[3] = { 122, 31, 18 };
const float GRAVITY = 0.5;
bool quit;
WindowList windows;
int *keyBinding = nullptr;
SDL_Renderer *ren = nullptr;
Button *tempButton = nullptr;
Window *tempWin = nullptr;

int main(int argc, char **argv) {

	//init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError();
		return 1;
	}

	//make the window
	SDL_Window *win = SDL_CreateWindow("YAMI Superalpha - Ver 0.0.4a", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		logSDLError();
		SDL_Quit();
		return 1;
	}
	SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);

	//make the renderer
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		logSDLError();
		SDL_Quit();
		return 1;
	}
	//SDL setup complete

	quit = false; //initialize misc. variables
	int mouseX, mouseY;
	int movingWinX = 0, movingWinY = 0;
	SDL_Event e;
	Player player;
	player.setTexture("world\\player.bmp", ren);
	time_t lastLeft = time(NULL);
	time_t lastRight = time(NULL);
	bool canSprint = true;
	Window *movingWindow = nullptr;
	void(*tempFunc)();
	Missile *t_missile = nullptr;

	windowInit(); //fill WindowList windows - windowInit.h

	MissileList missiles;

	BlockList level;
	level.setMapW(1500); //initialize level
	level.setMapH(540);
	level.add(200, level.getMapH() - 130, 50, ren);
	level.add(175, level.getMapH() - 120, 50, ren);
	level.add(150, level.getMapH() - 110, 50, ren);
	level.add(125, level.getMapH() - 100, 50, ren);
	level.add(100, level.getMapH() - 90, 50, ren);
	level.add(75, level.getMapH() - 80, 50, ren);
	level.add(50, level.getMapH() - 70, 50, ren);
	level.add(25, level.getMapH() - 60, 50, ren);
	level.add(0, level.getMapH() - 50, level.getMapW(), ren);
	
	while (!quit) { //game loop
		while (SDL_PollEvent(&e)) { //event polling
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					tempWin = windows.checkClick(e.button.x, e.button.y);
					if (tempWin != nullptr) {
						tempButton = tempWin->clickedOn(e.button.x, e.button.y);
						if (tempButton != nullptr) {
							tempFunc = tempButton->getFunction();
							tempFunc();
						} else if (tempWin->getMoving()) {
							movingWindow = tempWin;
							movingWinX = movingWindow->getxPos() - e.button.x;
							movingWinY = movingWindow->getyPos() - e.button.y;
						}
					}
				}
			}
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (movingWindow != nullptr) {
						movingWindow->setMoving(false);
						movingWindow = nullptr;
					}
				}
			}
			if (e.type == SDL_KEYDOWN) {
				if (keyBinding != nullptr) {
					if (windows.get(2)->getOpen()) {
						*keyBinding = e.key.keysym.sym;
						keyBinding = nullptr;
						windows.get(2)->toggleOpen();
					} else {
						keyBinding = nullptr;
					}
				}
				if (e.key.keysym.sym == KEYB_mainMenu) {
					windows.get(0)->toggleOpen();
				}
				if (e.key.keysym.sym == KEYB_jump) {
					if (player.getJumps()) {
						player.setyVel(-5);
						player.setJumps(player.getJumps() - 1);
					}
				}
				if (e.key.keysym.sym == KEYB_moveLeft) {
					if (difftime(time(NULL), lastLeft) < 0.01 && canSprint) {
						player.setxVel(-3);
						canSprint = false;
					} else {
						player.setxVel(-1);
					}
					time(&lastLeft);
				}
				if (e.key.keysym.sym == KEYB_moveRight) {
					if (difftime(time(NULL), lastRight) < 0.01 && canSprint) {
						player.setxVel(3);
						canSprint = false;
					} else {
						player.setxVel(1);
					}
					time(&lastRight);
				}
				if (e.key.keysym.sym == KEYB_spellOne) {
					t_missile = new Missile(0, 0);
					t_missile->setTexture("missiles\\magicMissile.bmp", ren);
					t_missile->setCreationTimer();
					t_missile->setxPos(player.getxPos());
					t_missile->setyPos(player.getyPos());
					t_missile->setWidth(19);
					t_missile->setHeight(35);
					missiles.add(t_missile);
					t_missile = nullptr;
				}
			}
			if (e.type == SDL_KEYUP) {
				if (e.key.keysym.sym == KEYB_moveLeft) {
					if (player.getxVel() < 0) {
						player.setxVel(0);
						canSprint = true;
					}
				}
				if (e.key.keysym.sym == KEYB_moveRight) {
					if (player.getxVel() > 0) {
						player.setxVel(0);
						canSprint = true;
					}
				}
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);
		player.addGravity(GRAVITY); //updating game state
		player.move(level.getMapW(), level.getMapH(), level);
		if (movingWindow != nullptr && movingWindow->getMoving()) {
			movingWindow->setxPos(mouseX + movingWinX);
			movingWindow->setyPos(mouseY + movingWinY);
		} else { movingWindow = nullptr; }
		t_missile = missiles.getHead();
		while(t_missile != nullptr) {
			t_missile->callFunction(mouseX, mouseY);
			if (t_missile->getLifetime() <= 0) {
				if (missiles.getLength() != 1) {
					t_missile = t_missile->getNext();
					missiles.remove(t_missile->getLast());
				} else {
					missiles.remove(t_missile);
					break;
				}
				continue;
			}
			t_missile = t_missile->getNext();
		}

		SDL_SetRenderDrawColor(ren, color1[0], color1[1], color1[2], 255); //updating graphical state
		SDL_RenderClear(ren); //set background color
		SDL_Rect rect;
		rect.w = level.getMapW();
		rect.h = level.getMapH();
		rect.x = (SCREEN_WIDTH / 2) - player.getxPos();
		rect.y = (SCREEN_HEIGHT / 2) - player.getyPos();
		SDL_SetRenderDrawColor(ren, color2[0], color2[1], color2[2], 255);
		SDL_RenderDrawRect(ren, &rect); // visually establish play area boundaries
		player.draw(ren, SCREEN_WIDTH, SCREEN_HEIGHT); //draw player sprite
		level.draw(ren, player.getxPos(), player.getyPos(), SCREEN_WIDTH, SCREEN_HEIGHT); //draw platforms
		t_missile = missiles.getHead();
		while (t_missile != nullptr) {
			t_missile->draw(ren, player.getxPos(), player.getyPos());
			t_missile = t_missile->getNext();
		}
		windows.draw(ren); //draw windows
		SDL_RenderPresent(ren); //render onto screen
	}

	level.empty(); //clean up lists
	missiles.empty();
	windows.empty();
	return 0;
}

void exitGame() {
	quit = true;
}
void toggleOptionsWin() {
	windows.get(1)->toggleOpen();
}
void setKeybindLeft() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_moveLeft;
}
void setKeybindRight() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_moveRight;
}
void setKeybindJump() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_jump;
}
void setKeybindSpellOne() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_spellOne;
}
void setKeybindSpellTwo() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_spellTwo;
}
void setKeybindSpellThree() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_spellThree;
}
void setKeybindSpellFour() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_spellFour;
}
void setKeybindMainMenu() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_mainMenu;
}
void setKeybindInventory() {
	windows.get(2)->toggleOpen();
	keyBinding = &KEYB_inventory;
}
