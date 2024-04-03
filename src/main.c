#include "./constants.h"
#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables
bool game_is_running = false;
int last_frame_time = 0;
unsigned int count = 1;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Create two game objects: ball and paddle
struct game_object {
  float x;
  float y;
  float width;
  float height;
  float vel_x;
  float vel_y;
} ball, paddle;

// Function to initialize SDL window and renderer
bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }
#ifdef __EMSCRIPTEN__
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window,
                              &renderer);
  if (window == NULL) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return false;
  } else if (renderer == NULL) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return false;
  }
#else
  window = SDL_CreateWindow("A simple game loop using C & SDL",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!window) {
    fprintf(stderr, "Error creating SDL Window.\n");
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL Renderer.\n");
    return false;
  }
#endif
  return true;
}

// Setup function to initialize the game objects
void setup(void) {
  // Reset count
  count = 1;

  // Seed random number generator
  srand(time(0));

  // Randomize ball position and set velocity
  ball.x = rand() % WINDOW_WIDTH;
  ball.y = 0;
  ball.width = 20;
  ball.height = 20;
  ball.vel_x = 140;
  ball.vel_y = 180;

  paddle.width = 100;
  paddle.height = 20;
  paddle.x = (WINDOW_WIDTH / 2.0) - (paddle.width / 2);
  paddle.y = WINDOW_HEIGHT - 40;
}

// Function to poll SDL events and process keyboard input
void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      // Quit game with escape key
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        game_is_running = false;
      }
      // Move paddle with left arrow keys
      if (event.key.keysym.sym == SDLK_LEFT) {
        paddle.x -= 10;
      }
      // Move paddle with right arrow keys
      if (event.key.keysym.sym == SDLK_RIGHT) {
        paddle.x += 10;
      }
      // Restart game with space key
      if (event.key.keysym.sym == SDLK_r) {
        setup();
      }
      break;
    }
  }
}

// Update function with a fixed time step
void update(void) {
  // Get delta time factor converted to seconds to be used to update objects
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  // Store the milliseconds of the current frame to be used in the next one
  last_frame_time = SDL_GetTicks();

  // Move ball as a function of delta time
  ball.x += ball.vel_x * delta_time;
  ball.y += ball.vel_y * delta_time;

  // Check for ball collision with the window borders
  if (ball.x < 0) {
    ball.x = 0;
    ball.vel_x = -ball.vel_x;
  }
  if (ball.x + ball.height > WINDOW_WIDTH) {
    ball.x = WINDOW_WIDTH - ball.width;
    ball.vel_x = -ball.vel_x;
  }
  if (ball.y < 0) {
    ball.y = 0;
    ball.vel_y = -ball.vel_y;
  }

  // End game if ball goes below the window height
  // if (ball.y + ball.height > WINDOW_HEIGHT) {
  //   game_is_running = false;
  // }

  // Check for paddle collision with the window borders
  if (paddle.x < 0) {
    paddle.x = 0;
  }
  if (paddle.x + paddle.width > WINDOW_WIDTH) {
    paddle.x = WINDOW_WIDTH - paddle.width;
  }

  // Check for ball collision with the paddle
  if (ball.y + ball.height > paddle.y && ball.y < paddle.y + paddle.height &&
      ball.x + ball.width > paddle.x && ball.x < paddle.x + paddle.width) {
    ball.y = paddle.y - ball.height;
    ball.vel_y = -ball.vel_y;
    ++count;
  }

  // Increase ball velocity every 5 hits
  if (count % 5 == 0) {
    ball.vel_x = ball.vel_x < 0 ? ball.vel_x - 0.01 : ball.vel_x + 0.01;
    ball.vel_y = ball.vel_y < 0 ? ball.vel_y - 0.01 : ball.vel_y + 0.01;
  }
}

// Render function to draw the game objects in SDL window
void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw a rectangle for the ball object
  SDL_Rect ball_rect = {(int)ball.x, (int)ball.y, (int)ball.width,
                        (int)ball.height};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);

  // Draw a rectangle for the paddle object
  SDL_Rect paddle_rect = {(int)paddle.x, (int)paddle.y, (int)paddle.width,
                          (int)paddle.height};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &paddle_rect);

  SDL_RenderPresent(renderer);
}

// Function to destroy SDL window and renderer
void destroy_window(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

// Function to run the main game loop
void main_loop() {
  process_input();
  update();
  render();
}

int main(int argc, char *argv[]) {
  game_is_running = initialize_window();

  if (!game_is_running) {
    destroy_window();
    return EXIT_FAILURE;
  }

  setup();

  while (game_is_running) {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, -1, 1);
#else
    main_loop();
#endif
  }

  destroy_window();
  return EXIT_SUCCESS;
}
