#pragma once
#include "Window.h"

extern WindowList windows;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern void exitGame();
extern void toggleOptionsWin();
extern void setKeybindLeft();
extern void setKeybindRight();
extern void setKeybindJump();
extern void setKeybindSpellOne();
extern void setKeybindSpellTwo();
extern void setKeybindSpellThree();
extern void setKeybindSpellFour();
extern void setKeybindMainMenu();
extern void setKeybindInventory();
extern SDL_Renderer *ren;
extern Window *tempWin;
extern Button *tempButton;

void windowInit() {
	Window *tempWin = new Window; //main menu
	tempWin->setTexture("windows\\mainMenu.bmp", ren);
	tempWin->setId(0);
	tempWin->setName("mainMenu");
	tempWin->setxPos((SCREEN_WIDTH - tempWin->getWidth()) / 2);
	tempWin->setyPos((SCREEN_HEIGHT - tempWin->getHeight()) / 2);
	tempButton = tempWin->addButton(15, 50, "buttons\\keybinds.bmp", ren);
	tempButton->setFunction(toggleOptionsWin);
	tempButton = tempWin->addButton(15, 100, "buttons\\exitGame.bmp", ren);
	tempButton->setFunction(exitGame);
	tempButton = nullptr;
	windows.add(tempWin);
	tempWin = nullptr;

	tempWin = new Window; //keybinds menu
	tempWin->setTexture("windows\\keybinds.bmp", ren);
	tempWin->setId(1);
	tempWin->setName("keybinds");
	tempWin->setxPos((SCREEN_WIDTH - tempWin->getWidth()) / 2);
	tempWin->setyPos((SCREEN_HEIGHT - tempWin->getHeight()) / 2);
	tempButton = tempWin->addButton(15, 50, "buttons\\keybindLeft.bmp", ren);
	tempButton->setFunction(setKeybindLeft);
	tempButton = tempWin->addButton(15, 100, "buttons\\keybindRight.bmp", ren);
	tempButton->setFunction(setKeybindRight);
	tempButton = tempWin->addButton(15, 150, "buttons\\keybindJump.bmp", ren);
	tempButton->setFunction(setKeybindJump);
	tempButton = tempWin->addButton(15, 200, "buttons\\keybindSpellOne.bmp", ren);
	tempButton->setFunction(setKeybindSpellOne);
	tempButton = tempWin->addButton(15, 250, "buttons\\keybindSpellTwo.bmp", ren);
	tempButton->setFunction(setKeybindSpellTwo);
	tempButton = tempWin->addButton(15, 300, "buttons\\keybindSpellThree.bmp", ren);
	tempButton->setFunction(setKeybindSpellThree);
	tempButton = tempWin->addButton(15, 350, "buttons\\keybindSpellFour.bmp", ren);
	tempButton->setFunction(setKeybindSpellFour);
	tempButton = tempWin->addButton(180, 50, "buttons\\keybindMainMenu.bmp", ren);
	tempButton->setFunction(setKeybindMainMenu);
	tempButton = tempWin->addButton(180, 100, "buttons\\keybindInventory.bmp", ren);
	tempButton->setFunction(setKeybindInventory);
	windows.add(tempWin);
	tempWin = nullptr;

	tempWin = new Window;
	tempWin->setTexture("windows\\keybindInput.bmp", ren);
	tempWin->setId(2);
	tempWin->setName("keybindInput");
	tempWin->setxPos((SCREEN_WIDTH - tempWin->getWidth()) / 2);
	tempWin->setyPos((SCREEN_HEIGHT - tempWin->getHeight()) / 2);
	windows.add(tempWin);
	tempWin = nullptr;
}