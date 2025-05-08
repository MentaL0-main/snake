#ifndef FOOD_H
#define FOOD_H

#include <vector>

class Food {
public:
    Food(int px, int py)
    : x{px}, y{py} {}

    void setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    std::vector<int> getPosition() {
        return {x, y};
    }

private:
    int x, y;

};

#endif // FOOD_H
