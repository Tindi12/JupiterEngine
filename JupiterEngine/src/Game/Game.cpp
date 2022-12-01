#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

 
Game::Game() {
	isRunning = false;
	Logger::Log("game constructor called...");
} 
                                                                                                                                                               
Game::~Game() {
	Logger::Log("game destructor called...");
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL...");
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
		 Logger::Err("Error creating SDL window...");
		return;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		Logger::Err("Error creating SDL renderer...");
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

 
void Game::Setup() {
	//Create entity(tank) = registry.CreateEntity();
}
   
void Game::Update() {
	//if too fast, lock execution until time passes, then release execution for next frame.
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	//the difference in ticks since the last frame, converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	//store the previous frame time
	millisecsPreviousFrame = SDL_GetTicks();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

void Game::Run() {
	Setup();
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