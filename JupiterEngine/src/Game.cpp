#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;
 
void Game::Setup() {
	//INITIALIZE GAME OBJECTS
	playerPosition = glm::vec2(10.0, 20.0);
	playerVelocity = glm::vec2(10.0, 5.0);
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

	playerPosition.x += playerVelocity.x * deltaTime;
	playerPosition.y += playerVelocity.y * deltaTime;
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	//RENDER ALL GAME OBJECTS
	//Loads Texture
	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	//destination of the rectangle for the texture...
	SDL_Rect dstRect = { 
		static_cast<int>(playerPosition.x),
		static_cast<int>(playerPosition.y),
		32,
		32 
	};
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);
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