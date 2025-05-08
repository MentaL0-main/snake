#include "snake.h"

void Snake::run(int w, int h, std::string c) {
    this->width = w;
    this->height = h;
    this->caption = c;

    init();
    mainloop();
    cleanup();
}

void Snake::init() {
    srand(time(NULL));

    dir = STOP;
    tileNum = 20;

    setargc:
    foodsNumber = random() % 10;
    if (foodsNumber == 0)
        goto setargc;

    rect = {
        width / tileNum,
        height / tileNum
    };

    int headPosX = rect.x * 5;
    int headPosY = rect.y * 5;

    body.push_back({
        headPosX,
        headPosY
    });
    
    body.push_back({
        headPosX + rect.x,
        headPosY
    });

    linesColor = {
        Uint8(random() % 255),
        Uint8(random() % 255),
        Uint8(random() % 255),
        255
    };

    initFoods();
    initWindow(caption, width, height);
    initRenderer(window);
}

void Snake::mainloop() {
    bool bQuit = false;
    SDL_Event event;

    while (!bQuit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                bQuit = true;
        }

        renderFoods();
        snakeRender();
        snakeMovement();

        SDL_SetRenderDrawColor(renderer, 4, 4, 4, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 160, 0, 0, 255);
        showFoods();

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        snakeShow();

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

        showGrid();

        SDL_RenderPresent(renderer);

        SDL_Delay(65);
    }
}

void Snake::cleanup() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Snake::initWindow(std::string cap, int w, int h) {
    window = SDL_CreateWindow(
        cap.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_SHOWN
    );

    if (!window)
        throw std::runtime_error(SDL_GetError());
}

void Snake::initRenderer(SDL_Window *win) {
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
        throw std::runtime_error(SDL_GetError());
}

void Snake::showGrid() {
    for (int x = 1; x < rect.x*width; ++x) {
        SDL_RenderDrawLine(
            renderer,
            x*rect.x, 0,
            x*rect.x, height
        );
    }

    for (int y = 1; y < rect.y*height; ++y) {
        SDL_RenderDrawLine(
            renderer,
            0, y*rect.y,
            width, y*rect.y
        );
    }

    SDL_RenderDrawLine(
        renderer,
        0,
        0,
        width,
        0
    );

    SDL_RenderDrawLine(
        renderer,
        0,
        0,
        0,
        height
    );

    SDL_RenderDrawLine(
        renderer,
        width-1,
        0,
        height-1,
        width-1
    );

    SDL_RenderDrawLine(
        renderer,
        0,
        height-1,
        width-1,
        height-1
    );
}

void Snake::snakeMovement() {
    static const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_A] && dir != RIGHT)
        dir = LEFT;

    else if (state[SDL_SCANCODE_D] && dir != LEFT)
        dir = RIGHT;

    else if (state[SDL_SCANCODE_W] && dir != DOWN)
        dir = UP;

    else if (state[SDL_SCANCODE_S] && dir != UP)
        dir = DOWN;
}

void Snake::snakeRender() {
    if (body[0].x < 0)
        body[0].x = width - rect.x;

    else if (body[0].x >= width)
        body[0].x = 0;

    else if (body[0].y < 0)
        body[0].y = height - rect.y;

    else if (body[0].y >= height)
        body[0].y = 0;

    for (int i = 1; i < body.size(); ++i) {
        if (body[i].x == body[0].x &&
            body[i].y == body[0].y)
            showGameOverScene();
    }

    switch (dir) {
        case LEFT:
            body.pop_back();
            body.push_front({
                body[0].x - rect.x,
                body[0].y
            });
        break;

        case RIGHT:
            body.pop_back();
            body.push_front({
                body[0].x + rect.x,
                body[0].y
            });
        break;

        case UP:
            body.pop_back();
            body.push_front({
                body[0].x,
                body[0].y - rect.y
            });
        break;

        case DOWN:
            body.pop_back();
            body.push_front({
                body[0].x,
                body[0].y + rect.y
            });
        break;
    }

    if (body.size() == 200)
        showGameOverScene();
}

void Snake::snakeShow() {
    int i = 0;

    SDL_Color currentColor;
    SDL_GetRenderDrawColor(
        renderer,
        &currentColor.r,
        &currentColor.g,
        &currentColor.b,
        &currentColor.a
    );

    for (auto& segment : body) {
        SDL_SetRenderDrawColor(
            renderer,
            currentColor.r,
            currentColor.g-i,
            currentColor.b,
            currentColor.a
        );

        SDL_Rect r = {
            segment.x,
            segment.y,
            rect.x,
            rect.y
        };

        SDL_RenderFillRect(renderer, &r);

        i+=3;
    }
}

void Snake::showGameOverScene() {
    
    SDL_Delay(5000);

    dir = STOP;

    std::cout << "Game Over!\n";
    std::cout << "score: " << score << std::endl;

    snakeReset();

}

SDL_Point Snake::getRandomTile() {
    SDL_Point position;

    position.x = (random() % tileNum) * rect.x;
    position.y = (random() % tileNum) * rect.y;

    return position;
}

void Snake::snakeReset() {
    body.clear();

    body.push_back({
        12*rect.x,
        12*rect.y
    });

    body.push_back({
        13*rect.x,
        12*rect.y
    });

    score = 2;

    initFoods();
}

void Snake::showFoods() {
    for (auto& food : foods) {
        std::vector<int> position = food.getPosition();
        
        SDL_Rect rectr = {
            position[0],
            position[1],
            rect.x,
            rect.y
        };
        
        SDL_RenderFillRect(renderer, &rectr);
    }
}

void Snake::renderFoods() {
    for (auto& food : foods) {
        std::vector<int> p = food.getPosition();
        if (body[0].x == p[0] && body[0].y == p[1]) {
            ++score;

            if (dir == LEFT)
                body.push_back({
                    body[body.size()-1].x + rect.x,
                    body[body.size()-1].y
                });

            else if (dir == RIGHT)
                body.push_back({
                    body[body.size()-1].x - rect.x,
                    body[body.size()-1].y
                });

            else if (dir == UP)
                body.push_back({
                    body[body.size()-1].x,
                    body[body.size()-1].y + rect.y
                });

            else if (dir == DOWN)
                body.push_back({
                    body[body.size()-1].x,
                    body[body.size()-1].y - rect.y
                });


            respawn2:
            SDL_Point newPosition = getRandomTile();

            // check collide by other foods
            for (auto& food : foods) {
                if (food.getPosition()[0] == newPosition.x &&
                    food.getPosition()[1] == newPosition.y)
                    goto respawn2;
            }

            // check collide by snake segments
            for (auto& segment : body) {
                if (newPosition.x == segment.x &&
                    newPosition.y == segment.y)
                    goto respawn2;
            }

            food.setPosition(newPosition.x, newPosition.y);

            break;
        }
    }
}

void Snake::initFoods() {
    foods.clear();

    for (int i = 0; i < foodsNumber; ++i) {
        respawn:

        SDL_Point position = getRandomTile();

        // check collide by other food
        for (auto& food : foods) {
            std::vector<int> p = food.getPosition();
            if (p[0] == position.x && p[1] == position.y) {
                goto respawn;
            }
        }

        // check collide by snake segments
        for (auto& segment : body) {
            if (segment.x == position.x && segment.y == position.y) {
                goto respawn;
            }
        }

        foods.push_back({position.x, position.y});
    }
}