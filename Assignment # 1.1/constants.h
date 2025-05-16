#pragma once

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define DISPLAY_SCALE 2

enum ObjectType
{
	TYPE_BALL,
	TYPE_BRICK
};

const float BALL_SPEED = 5.0f;
const int PADDLE_WIDTH = 60;
const int PADDLE_HEIGHT = 10;
const int PADDLE_Y = DISPLAY_HEIGHT - 20;
const int PADDLE_SPEED = 5;
