#pragma once
#include "SDL3/SDL.h"
#include <vector>

struct Vector2
{
	float x;
	float y;
};

struct Ball {
	Vector2 position;
	Vector2 velocity;
};

class Game {
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	const int thickness = 15;
	const float paddleH = 100.0f;
	int mPaddleDir;
	int mPaddle2Dir;

	std::vector<Ball> balls;
	Vector2 mBallPos; 
	Vector2 mPaddlePos; 
	Vector2 mPaddle2Pos; 
	Vector2 mBallVel;

};