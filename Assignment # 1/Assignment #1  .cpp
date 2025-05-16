#include "Play.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int BALL_ID = 1;
constexpr int BRICK_ID_START = 100;
constexpr int BRICK_COUNT = 20;

int currentScore = 0;
unsigned int* highScores = nullptr;
int highScoreCount = 0;

const char* HIGH_SCORE_FILE = "highscores.txt";

// Function declarations
void LoadHighScores();
void SaveHighScores();
void InsertHighScore(unsigned int score);
void CleanUp();
void ResetGame();
void DrawScores();
void SpawnBricks();
void SpawnBall();

void MainGameUpdate(float deltaTime)
{
    Play::ClearDrawingBuffer(Play::cWhite);

    // Ball movement
    GameObject& ball = Play::GetGameObject(BALL_ID);
    ball.pos += ball.velocity * deltaTime;

    // Wall collision
    if (ball.pos.x < 0 || ball.pos.x > DISPLAY_WIDTH)
        ball.velocity.x *= -1;
    if (ball.pos.y < 0)
        ball.velocity.y *= -1;

    // Ball lost
    if (ball.pos.y > DISPLAY_HEIGHT)
    {
        InsertHighScore(currentScore);
        ResetGame();
        return;
    }

    // Collision with bricks
    std::vector<int> bricks = Play::CollectGameObjectIDsByType("brick");
    for (int id : bricks)
    {
        if (Play::IsColliding(BALL_ID, id))
        {
            Play::DestroyGameObject(id);
            ball.velocity.y *= -1;
            currentScore++;
            break;
        }
    }

    // Draw everything
    Play::DrawGameObject(BALL_ID);
    for (int id : bricks)
        Play::DrawGameObject(id);

    DrawScores();

    Play::PresentDrawingBuffer();
}

void DrawScores()
{
    // Bottom left for current score
    Play::DrawDebugText({ 10, DISPLAY_HEIGHT - 30 }, "Score: " + std::to_string(currentScore), Play::cBlack);

    // Bottom right for high scores
    int x = DISPLAY_WIDTH - 200;
    int y = DISPLAY_HEIGHT - 150;
    for (int i = 0; i < highScoreCount; ++i)
    {
        Play::DrawDebugText({ x, y + i * 20 }, "High " + std::to_string(i + 1) + ": " + std::to_string(highScores[i]), Play::cBlue);
    }
}

void InsertHighScore(unsigned int score)
{
    std::vector<unsigned int> scoresVec(highScores, highScores + highScoreCount);
    scoresVec.push_back(score);
    std::sort(scoresVec.begin(), scoresVec.end(), std::greater<>());

    // Keep only top N
    if (scoresVec.size() > 5)
        scoresVec.resize(5);

    delete[] highScores;
    highScoreCount = scoresVec.size();
    highScores = new unsigned int[highScoreCount];

    for (int i = 0; i < highScoreCount; ++i)
        highScores[i] = scoresVec[i];
}

void LoadHighScores()
{
    std::ifstream file(HIGH_SCORE_FILE);
    if (!file.is_open()) return;

    std::vector<unsigned int> tempScores;
    unsigned int val;
    while (file >> val)
        tempScores.push_back(val);

    highScoreCount = tempScores.size();
    highScores = new unsigned int[highScoreCount];
    for (int i = 0; i < highScoreCount; ++i)
        highScores[i] = tempScores[i];

    file.close();
}

void SaveHighScores()
{
    std::ofstream file(HIGH_SCORE_FILE);
    if (!file.is_open()) return;

    for (int i = 0; i < highScoreCount; ++i)
        file << highScores[i] << "\n";

    file.close();
}

void CleanUp()
{
    SaveHighScores();
    delete[] highScores;
}

void ResetGame()
{
    // Reset score
    currentScore = 0;

    // Destroy bricks and ball
    for (int id : Play::CollectGameObjectIDsByType("brick"))
        Play::DestroyGameObject(id);

    Play::DestroyGameObject(BALL_ID);

    // Respawn
    SpawnBricks();
    SpawnBall();
}

void SpawnBricks()
{
    for (int i = 0; i < BRICK_COUNT; ++i)
    {
        int x = 100 + (i % 10) * 100;
        int y = 100 + (i / 10) * 50;
        int id = Play::CreateGameObject("brick", { x, y }, 20, "brick.png");
        Play::SetGameObjectDirection(id, { 0, 0 });
    }
}

void SpawnBall()
{
    Play::CreateGameObject("ball", { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 100 }, 10, "ball.png", BALL_ID);
    GameObject& ball = Play::GetGameObject(BALL_ID);
    ball.velocity = { 300, -300 };
}

void MainGameEntry()
{
    Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Breakout Game");
    LoadHighScores();
    ResetGame();
}

bool MainGameExit()
{
    CleanUp();
    return true;
}
