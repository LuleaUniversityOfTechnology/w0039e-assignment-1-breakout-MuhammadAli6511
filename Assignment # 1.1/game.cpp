#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "game.h"
#include "constants.h"

void SpawnBall()
{
	int id = Play::CreateGameObject(TYPE_BALL, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 60 }, 4, "ball");
	GameObject& ball = Play::GetGameObject(id);
	ball.velocity = Play::Normalize({ 1, -1 }) * BALL_SPEED;
}

void SetupScene()
{
	for (int y = 40; y < 100; y += 20)
	{
		for (int x = 40; x < DISPLAY_WIDTH - 40; x += 40)
		{
			Play::CreateGameObject(TYPE_BRICK, { x, y }, 6, "brick");
		}
	}
}

void StepFrame(float elapsedTime)
{
	std::vector<int> ballIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	std::vector<int> brickIds = Play::CollectGameObjectIDsByType(TYPE_BRICK);

	for (int id : brickIds)
	{
		GameObject& brick = Play::GetGameObject(id);
		Play::DrawObject(brick);
	}

	for (int id : ballIds)
	{
		GameObject& ball = Play::GetGameObject(id);

		// Bounce on borders
		if (ball.pos.x <= 0 || ball.pos.x >= DISPLAY_WIDTH) ball.velocity.x *= -1;
		if (ball.pos.y <= 0 || ball.pos.y >= DISPLAY_HEIGHT) ball.velocity.y *= -1;

		// Collisions with bricks
		for (int bid : brickIds)
		{
			GameObject& brick = Play::GetGameObject(bid);
			if (Play::IsColliding(ball, brick))
			{
				Play::DestroyGameObject(bid);
				ball.velocity.y *= -1;
				break;
			}
		}

		Play::UpdateGameObject(ball, elapsedTime);
		Play::DrawObject(ball);
	}
}
