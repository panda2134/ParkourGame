#ifndef CONSTS_H
#define CONSTS_H

namespace parkour {
const int TICKS_PER_SEC = 60;
const double TICK_LENGTH = 1.0 / TICKS_PER_SEC;
const double EPSILON = 1e-3;
/**
 * @brief MAX_VELOCITY 速度分量的最大值
 */
const double MAX_VELOCITY = 50;
/**
 * @brief MAX_VELOCITY 加速度分量的最大值
 */
const double MAX_ACCELERATION = 20;
/**
 * @brief ENTITY_UPDATE_RANGE 实体更新范围，用于碰撞计算、实体ai寻路等
 */
const double ENTITY_UPDATE_RANGE = 32;
/**
 * @brief DYING_ANIMATION_TICKS 死亡动画渲染tick数目
 */
const double DYING_ANIMATION_TICKS = 60;
}

#endif // CONSTS_H
