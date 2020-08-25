#ifndef CONSTS_H
#define CONSTS_H

namespace parkour {
const int TICKS_PER_SEC = 60;
const float TICK_LENGTH = 1.0 / TICKS_PER_SEC;
const float EPSILON = 1e-3;
/**
 * @brief MAX_VELOCITY 速度分量的最大值
 */
const float MAX_VELOCITY = 10;
/**
 * @brief MAX_VELOCITY 加速度分量的最大值
 */
const float MAX_ACCELERATION = 10;
}

#endif // CONSTS_H
