#include "paddle.h"

Paddle paddle = { { DISPLAY_WIDTH / 2, PADDLE_Y } };

void DrawPaddle(const Paddle& p)
{
	Point2f topLeft = { p.pos.x - PADDLE_WIDTH / 2, p.pos.y - PADDLE_HEIGHT / 2 };
	Point2f bottomRight = { p.pos.x + PADDLE_WIDTH / 2, p.pos.y + PADDLE_HEIGHT / 2 };
	Play::DrawRect(topLeft, bottomRight, Play::cWhite);
}

void UpdatePaddle(Paddle& p)
{
	if (Play::KeyDown(VK_LEFT)) p.pos.x -= PADDLE_SPEED;
	if (Play::KeyDown(VK_RIGHT)) p.pos.x += PADDLE_SPEED;
}

float Max(float a, float b) { return a > b ? a : b; }
float Min(float a, float b) { return a < b ? a : b; }

bool IsColliding(const Paddle& p, const GameObject& obj)
{
	Point2f topLeft = { p.pos.x - PADDLE_WIDTH / 2, p.pos.y - PADDLE_HEIGHT / 2 };
	Point2f bottomRight = { p.pos.x + PADDLE_WIDTH / 2, p.pos.y + PADDLE_HEIGHT / 2 };

	float dx = obj.pos.x - Max(topLeft.x, Min(obj.pos.x, bottomRight.x));
	float dy = obj.pos.y - Max(topLeft.y, Min(obj.pos.y, bottomRight.y));
	return (dx * dx + dy * dy) < (obj.radius * obj.radius);
}
