#include "block.h"

namespace parkour {

BoundingBox Block::getBoundingBox() const {
    BoundingBox ret;
    ret.offset = QVector2D(.0, .0);
    ret.dimensions = QVector2D(1., 1.);
    return ret;
}

}
