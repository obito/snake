#include "raylib.h"
#include <stdio.h>

static const int screenWidth = 800;
static const int screenHeight = 450;
static int framesCounter = 0;

#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

typedef struct Snake {
  Vector2 pos;
  Vector2 speed;
  Vector2 size;
  Color color;
} Snake;

typedef struct Food {
  Vector2 pos;
  Vector2 size;
  bool active;
  Color color;
} Food;

static bool gameOver = false;
static Food fruit = {0};
static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0};
static Vector2 offset = {0};
static int counterTail = 0;
static bool allowMove = false;

void UpdateDrawFrame(void); // Update and Draw one frame
void InitGame(void);

int main() {

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  InitGame();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }

  CloseWindow();
  return 0;
}

void InitGame(void) {
  gameOver = false;

  counterTail = 1;
  offset.x = screenWidth % SQUARE_SIZE;
  offset.y = screenHeight % SQUARE_SIZE;

  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake[i].pos = (Vector2){offset.x / 2, offset.y / 2};
    snake[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    snake[i].speed = (Vector2){SQUARE_SIZE, 0};

    if (i == 0)
      snake[i].color = DARKBLUE;
    else
      snake[i].color = BLUE;

    snakePosition[i] = (Vector2){0.0f, 0.0f};
  }

  fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
  fruit.color = SKYBLUE;
  fruit.active = false;
}

void UpdateGame(void) {
  if (gameOver) {
    if (IsKeyPressed(KEY_ENTER)) {
      InitGame();
      gameOver = false;
    }
  }

  if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove) {
    snake[0].speed = (Vector2){-SQUARE_SIZE, 0};
    allowMove = false;
  }

  if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove) {
    snake[0].speed = (Vector2){SQUARE_SIZE, 0};
    allowMove = false;
  }

  if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove) {
    snake[0].speed = (Vector2){0, -SQUARE_SIZE};
    allowMove = false;
  }

  if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove) {

    snake[0].speed = (Vector2){0, SQUARE_SIZE};
    allowMove = false;
  }

  for (int i = 0; i < counterTail; i++)
    snakePosition[i] = snake[i].pos;

  if ((framesCounter % 5) == 0) {
    for (int i = 0; i < counterTail; i++) {
      if (i == 0) {
        snake[0].pos.x += snake[0].speed.x;
        snake[0].pos.y += snake[0].speed.y;
        allowMove = true;
      } else
        snake[i].pos = snakePosition[i - 1];
    }
  }

  // Wall behavior
  // right side collision
  if (((snake[0].pos.x) > (screenWidth - offset.x))) {
    snake[0].pos.x = offset.x / 2;
  }

  // bottom collision
  if (((snake[0].pos.y) > (screenHeight - offset.y))) {
    snake[0].pos.y = offset.y / 2;
  }

  // left side collision
  if (snake[0].pos.x < offset.x / 2) {
    snake[0].pos.x = (screenWidth - offset.x / 2) - SQUARE_SIZE;
  }

  // top collision
  if (snake[0].pos.y < offset.y / 2) {
    snake[0].pos.y = (screenHeight - offset.y / 2) - SQUARE_SIZE;
  }

  for (int i = 1; i < counterTail; i++) {
    if ((snake[0].pos.x == snake[i].pos.x) &&
        (snake[0].pos.y == snake[i].pos.y))
      gameOver = true;
  }

  if (!fruit.active) {
    fruit.active = true;
    fruit.pos = (Vector2){
        GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE +
            offset.x / 2,
        GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE +
            offset.y / 2};
  }

  if ((snake[0].pos.x == fruit.pos.x) && (snake[0].pos.y == fruit.pos.y)) {
    snake[counterTail].pos = snakePosition[counterTail - 1];
    counterTail += 1;
    fruit.active = false;
  }

  framesCounter++;
}

void UpdateDrawFrame(void) {
  UpdateGame();
  BeginDrawing();

  ClearBackground(RAYWHITE);

  if (gameOver) {
    DrawText("PRESS [ENTER] TO PLAY AGAIN",
             GetScreenWidth() / 2 -
                 MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
             GetScreenHeight() / 2 - 50, 20, GRAY);
    EndDrawing();
    return;
  }

  for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++) {
    DrawLineV(
        (Vector2){SQUARE_SIZE * i + offset.x / 2, offset.y / 2},
        (Vector2){SQUARE_SIZE * i + offset.x / 2, screenHeight - offset.y / 2},
        LIGHTGRAY);
  }

  for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++) {
    DrawLineV(
        (Vector2){offset.x / 2, SQUARE_SIZE * i + offset.y / 2},
        (Vector2){screenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2},
        LIGHTGRAY);
  }

  for (int i = 0; i < counterTail; i++)
    DrawRectangleV(snake[i].pos, snake[i].size, snake[i].color);

  DrawRectangleV(fruit.pos, fruit.size, RED);
  EndDrawing();
}
