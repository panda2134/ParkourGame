#ifndef DIRECTION_H
#define DIRECTION_H

namespace parkour {
enum Direction {
    UP, // 0
    DOWN, // 1
    LEFT, // 2
    RIGHT, // 3
    UNKNOWN
};

Direction getOppositeFace(Direction face);
}

#endif // DIRECTION_H
