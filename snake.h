#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <exception>
#include <string>
#include <deque>

#include "food.h"

class Snake {
public:
    void run(int width, int height, std::string caption);

private:
    SDL_Window* window_{};
    SDL_Renderer* renderer_{};

    std::string caption_{};

    int width_{}, height_{};
    int score{1};
    int foodsNumber{};
    int tileNum{};

    short dir{};
    
    enum {
        STOP = 0,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    SDL_Rect rect{};
    SDL_Color linesColor{};

    std::deque<SDL_Point> body{};
    std::deque<Food> foods{};

    void init();
    void mainloop();
    void cleanup();

    void initWindow(std::string caption, int width, int height);
    void initRenderer(SDL_Window* window);

    void snakeMovement();
    void snakeRender();
    void snakeShow();
    void snakeReset();

    void showGrid();
    void showGameOverScene();
    void showFoods();

    void renderFoods();
    void initFoods();

    void showScoreOnTitle();

    SDL_Point getRandomTile();

};

#endif // SNAKE_H
