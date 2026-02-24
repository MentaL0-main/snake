#ifndef FOOD_H
#define FOOD_H

#include <vector>

class Food {
public:
    Food(int x, int y)
    : x_{x}, y_{y} {}

    void setPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }

    std::vector<int> getPosition() {
        return {x_, y_};
    }

private:
    int x_, y_;
};

#endif // FOOD_H
