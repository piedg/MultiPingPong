#include "Game.h"

Game::Game():
	mWindow(nullptr), 
	mIsRunning(true),
	mRenderer(nullptr),
	mTicksCount(0),
	mPaddleDir(0),
	mPaddle2Dir(0)
{
}

bool Game::Initialize()
{
	bool sdlSuccess = SDL_Init(SDL_INIT_VIDEO);

	if (!sdlSuccess) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming C++ Book Chapter 1",
		1024,
		768,
		0);

	if (!mWindow) {
		SDL_Log("Unable to create SDL window: %s", SDL_GetError());
		return false;
	}
	
	mRenderer = SDL_CreateRenderer(
		mWindow,
		NULL);
	SDL_SetRenderVSync(mRenderer, -1);

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mPaddle2Pos.x = 1024.0f - 24.f;
	mPaddle2Pos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	Ball* ball1 = new Ball();
	ball1->position.x = 1024.0f / 2.0f;
	ball1->position.y = 768.0f / 2.0f;
	ball1->velocity.x = -200.0f;
	ball1->velocity.y = 235.0f;

	Ball* ball2 = new Ball();
	ball2->position.x = 1024.0f / 2.0f;
	ball2->position.y = 768.0f / 2.0f;
	ball2->velocity.x = 200.0f;
	ball2->velocity.y = -235.0f;

	Ball* ball3 = new Ball();
	ball3->position.x = 800.0f / 2.0f;
	ball3->position.y = 800.0f / 2.0f;
	ball3->velocity.x = -200.0f;
	ball3->velocity.y = -235.0f;

	Ball* ball4 = new Ball();
	ball4->position.x = 1024.0f / 2.0f;
	ball4->position.y = 200.0f / 2.0f;
	ball4->velocity.x = 200.0f;
	ball4->velocity.y = 235.0f;

	balls.push_back(*ball1);
	balls.push_back(*ball2);
	//balls.push_back(*ball3);
	//balls.push_back(*ball4);
	

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	const bool* keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddleDir = 0;
	if (keyboardState[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	mPaddle2Dir = 0;
	if (keyboardState[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (keyboardState[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
	}
}

void Game::UpdateGame()
{
	Uint64 now = SDL_GetTicks();

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
	
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;

		if (mPaddle2Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddle2Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	for (int i = 0; i < balls.size(); i++)
	{
	
		balls[i].position.x += balls[i].velocity.x * deltaTime;
		balls[i].position.y += balls[i].velocity.y * deltaTime;

		float diff = mPaddlePos.y - balls[i].position.y;
		diff = (diff > 0.0f) ? diff : -diff;
		if (diff <= paddleH / 2.0f &&
			balls[i].position.x <= 25.f && balls[i].position.x >= 20.0f &&
			balls[i].velocity.x < 0.0f) {
			balls[i].velocity.x *= -1.0f;
		}

		float diff2 = mPaddle2Pos.y - balls[i].position.y;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
		if (diff2 <= paddleH / 2.0f &&
			balls[i].position.x <= 1024.f && balls[i].position.x >= 1000.0f &&
			balls[i].velocity.x > 0.0f) {
			balls[i].velocity.x *= -1.0f;
		}

		if (balls[i].position.y <= thickness && balls[i].velocity.y < 0.0f) {
			balls[i].velocity.y *= -1;
		}
		else if (balls[i].position.x <= 0.0f || balls[i].position.x >= 1024.0f)
		{
			mIsRunning = false;
		}


		if (balls[i].position.y <= thickness && balls[i].velocity.y < 0.0f)
		{
			balls[i].velocity.y *= -1;
		}
		else if (balls[i].position.y >= (768 - thickness) &&
			balls[i].velocity.y > 0.0f)
		{
			balls[i].velocity.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255);
	SDL_RenderClear(mRenderer);

	// Draw Game Scene Here
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_FRect wallTop{
		0,
		0,
		1024,
		thickness 
	};
	SDL_RenderFillRect(mRenderer, &wallTop);

	SDL_FRect wallBottom{
		0,
		768 - thickness,
		1024,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &wallBottom);

	SDL_FRect wallLeft{
		0,
		0,
		thickness,
		1024
	};
	//SDL_RenderFillRect(mRenderer, &wallLeft);

	SDL_FRect wallRight{
		1024 - thickness,
		0,
		thickness,
		1024
	};
	//SDL_RenderFillRect(mRenderer, &wallRight);

	SDL_FRect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_FRect paddle2{
	static_cast<int>(mPaddle2Pos.x),
	static_cast<int>(mPaddle2Pos.y - paddleH / 2),
	thickness,
	static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);

	for (int i = 0; i < balls.size(); i++)
	{
		Ball ball = balls[i];
		SDL_FRect ballRender{
			static_cast<int>(ball.position.x - thickness / 2),
			static_cast<int>(ball.position.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ballRender);
	}


	SDL_RenderPresent(mRenderer);
}
