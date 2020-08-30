#include "direction.h"

namespace parkour {

Direction getOppositeFace(Direction face) {
    return face != Direction::UNKNOWN ? static_cast<Direction>(static_cast<int>(face) ^ 1) : UNKNOWN;
}

}
