#ifndef CONSTS_H
#define CONSTS_H

namespace parkour {
const int TICKS_PER_SEC = 300;
const double TICK_LENGTH = 1.0 / TICKS_PER_SEC;
const int TICKS_PER_FRAME = 5;
const double EPSILON = 1e-5;
const double STAND_UPON_DELTA = 0.05;
const double GRAVITY = 9.854;
/**
 * @brief MAX_VELOCITY 速度分量的最大值
 */
const double MAX_VELOCITY = 50;
/**
 * @brief MAX_VELOCITY 加速度分量的最大值
 */
const double MAX_ACCELERATION = 20;
/**
 * @brief ENTITY_UPDATE_RANGE 实体碰撞计算更新范围
 */
const double ENTITY_COLLISION_RANGE = 8;
/**
 * @brief DYING_ANIMATION_TICKS 死亡动画渲染tick数目
 */
const double DYING_ANIMATION_TICKS = TICKS_PER_SEC * 0.5;
/**
 * @brief WORLD_HEIGHT 世界高度
 */
const int WORLD_HEIGHT = 16;
/**
 * @brief BLOCK_TEXTURE_SIZE 方块像素数目
 */
const int BLOCK_TEXTURE_SIZE = 16;
/**
 * @brief TEXTURE_MAP_SIZE 材质图像（拼接各个方块得到）的大小
 */
const int TEXTURE_MAP_SIZE = 512;
/**
 * @brief FALL_DAMAGE_MULTIPLIER 摔落伤害 / 纵向速度
 */
const double FALL_DAMAGE_MULTIPLIER = .5;
/**
 * @brief FALL_DAMAGE_SPEED 速率大于此数字时开始计算摔落伤害
 */
const double FALL_DAMAGE_SPEED = 10.0;
/**
 * @brief JUMP_SPEED 跳跃时纵向初速度大小
 */
const double JUMP_SPEED = 8.0;

const double EXPLOSION_DAMAGE_MULTIPLIER = 3.0;

const double EXPLOSION_RADIUS_MULTIPLIER = 1.0;
}

#endif // CONSTS_H
