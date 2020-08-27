#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "playerlikeentity.h"

namespace parkour {

class PlayerEntity : public PlayerLikeEntity {
    Q_OBJECT
public:
    PlayerEntity();
};
}

#endif // PLAYERENTITY_H
