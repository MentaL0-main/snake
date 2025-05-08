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
    void run(int w, int h, std::string c);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::string caption;

    int width, height;
    int score;
    int foodsNumber;
    int tileNum;

    short dir;
    
    enum {
        STOP = 0,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    SDL_Rect rect;
    SDL_Color linesColor;

    std::deque<SDL_Point> body;
    std::deque<Food> foods;

    void init();
    void mainloop();
    void cleanup();

    void initWindow(std::string cap, int w, int h);
    void initRenderer(SDL_Window *win);

    void snakeMovement();
    void snakeRender();
    void snakeShow();
    void snakeReset();

    void showGrid();
    void showGameOverScene();
    void showFoods();

    void renderFoods();
    void initFoods();

    SDL_Point getRandomTile();

};

#endif // SNAKE_H
