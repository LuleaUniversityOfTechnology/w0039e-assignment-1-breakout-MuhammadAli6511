#include "game.h"
#include "paddle.h"

void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	Play::LoadBackground("Data\\Backgrounds\\background.png");
	Play::LoadSprite("ball", "Data\\Sprites\\ball.png", 1);
	Play::LoadSprite("brick", "Data\\Sprites\\brick.png", 1);
	SetupScene();
	SpawnBall();
}

bool MainGameUpdate(float elapsedTime)
{
	Play::ClearDrawing(Play::cBlack);

	UpdatePaddle(paddle);
	DrawPaddle(paddle);
	StepFrame(elapsedTime);

	// Check paddle-ball collisions
	std::vector<int> balls = Play::CollectGameObjectIDsByType(TYPE_BALL);
	for (int id : balls)
	{
		GameObject& ball = Play::GetGameObject(id);
		if (IsColliding(paddle, ball)) ball.velocity.y *= -1;
	}

	Play::PresentDrawing();
	return Play::KeyDown(VK_ESCAPE) == false;
}

int MainGameExit()
{
	Play::DestroyManager();
	return PLAY_OK;
}
