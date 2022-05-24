// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MainMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

MainMenu::MainMenu(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EMainMenu);
	SetRelativeMouseMode(false);
	SetTitle("MainMenuTitle");
	AddButton("PlayButton", [this]() {
		Close();
	});
	AddButton("QuitButton", [this]() { 
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::EQuit);
		});
	});
}

MainMenu::~MainMenu()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void MainMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
}
