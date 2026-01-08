#include "snake.h"

int main() {
    Snake snake;

    try {
        snake.run(500, 500, "B-Snake");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
