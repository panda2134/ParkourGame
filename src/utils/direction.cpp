#include "direction.h"

namespace parkour {

Direction getOppositeFace(Direction face) {
    return static_cast<Direction>(static_cast<int>(face) ^ 1);
}

}
