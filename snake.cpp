#include "snake.h"

void Snake::run(int width, int height, std::string caption) {
    width_ = width;
    height_ = height;
    caption_ = caption;

    init();
    mainloop();
    cleanup();
}

void Snake::init() {
    srand(time(NULL));

    dir = STOP;
    tileNum = 20;

    setargc:
    foodsNumber = (random() % 24) + 1;
    
    rect = {
        width_ / tileNum,
        height_ / tileNum
    };

    int headPosX = rect.x * 5;
    int headPosY = rect.y * 5;

    body.push_back({
        headPosX,
        headPosY
    });

    body.push_back({
        headPosX+rect.x,
        headPosY
    });

    linesColor = {
        Uint8(random() % 255),
        Uint8(random() % 255),
        Uint8(random() % 255),
        255
    };

    initFoods();
    initWindow(caption_, width_, height_);
    initRenderer(window_);
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
        showScoreOnTitle();
        snakeRender();
        snakeMovement();

        SDL_SetRenderDrawColor(renderer_, 4, 4, 4, 255);
        SDL_RenderClear(renderer_);

        SDL_SetRenderDrawColor(renderer_, 160, 0, 0, 255);
        showFoods();

        SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
        snakeShow();

        SDL_SetRenderDrawColor(renderer_, 20, 20, 20, 255);

        showGrid();

        SDL_RenderPresent(renderer_);

        SDL_Delay(65);
    }
}

void Snake::cleanup() {
    if (window_) SDL_DestroyWindow(window_);
    if (renderer_) SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}

void Snake::initWindow(std::string caption, int width, int height) {
    window_ = SDL_CreateWindow(
        caption.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!window_)
        throw std::runtime_error(SDL_GetError());
}

void Snake::initRenderer(SDL_Window* window) {
    renderer_ = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer_)
        throw std::runtime_error(SDL_GetError());
}

void Snake::showGrid() {
    for (int x = 1; x < rect.x*width_; ++x) {
        SDL_RenderDrawLine(
            renderer_,
            x*rect.x, 0,
            x*rect.x, height_
        );
    }

    for (int y = 1; y < rect.y*height_; ++y) {
        SDL_RenderDrawLine(
            renderer_,
            0, y*rect.y,
            width_, y*rect.y
        );
    }

    SDL_RenderDrawLine(
        renderer_,
        0,
        0,
        width_,
        0
    );

    SDL_RenderDrawLine(
        renderer_,
        0,
        0,
        0,
        height_
    );

    SDL_RenderDrawLine(
        renderer_,
        width_-1,
        0,
        height_-1,
        width_-1
    );

    SDL_RenderDrawLine(
        renderer_,
        0,
        height_-1,
        width_-1,
        height_-1
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
        body[0].x = width_ - rect.x;

    else if (body[0].x >= width_)
        body[0].x = 0;

    else if (body[0].y < 0)
        body[0].y = height_ - rect.y;

    else if (body[0].y >= height_)
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
        renderer_,
        &currentColor.r,
        &currentColor.g,
        &currentColor.b,
        &currentColor.a
    );

    for (auto& segment : body) {
        SDL_SetRenderDrawColor(
            renderer_,
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

        SDL_RenderFillRect(renderer_, &r);

        i+=1;
    }
}

void Snake::showGameOverScene() {
    SDL_Delay(500);

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
        
        SDL_RenderFillRect(renderer_, &rectr);
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

void Snake::showScoreOnTitle() {
    std::string newTitle = 
        "Snake (" + std::to_string(this->score) + ")";

    SDL_SetWindowTitle(window_, newTitle.c_str());
}
