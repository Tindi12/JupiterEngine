#include "Game.h"
#include <SDL.h>
#include <iostream>


Game::Game() {
	isRunning = false;
	std::cout << "game constructor called" << std::endl;
}

Game::~Game() {
	std::cout << "game destructor called" << std::endl;
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;

	SDL_Window* window = SDL_CreateWindow("Jupiter Engine", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 
		windowWidth, 
		windowHeight, 
		SDL_WINDOW_BORDERLESS 
	);

	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
			//CASES FOR EVENTS IN WINDOW
		case SDL_QUIT: 
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false; 
			}
			break;
		}
	}
}

void Game::Update() {
	//UPDATE GAME OBJECTS
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	//RENDER ALL GAME OBJECTS

	SDL_RenderPresent(renderer);
}

void Game::Run() {
	while (isRunning){
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}