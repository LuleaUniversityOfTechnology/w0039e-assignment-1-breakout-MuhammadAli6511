#pragma once

#include "Play.h"
#include "constants.h"

struct Paddle
{
	Point2f pos;
};

extern Paddle paddle;

void DrawPaddle(const Paddle& p);
void UpdatePaddle(Paddle& p);
bool IsColliding(const Paddle& p, const GameObject& obj);
