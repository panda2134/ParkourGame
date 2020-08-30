#ifndef CONSTS_H
#define CONSTS_H

namespace parkour {
/**
 * @brief TICKS_PER_SEC 每秒游戏刻数目
 */
const int TICKS_PER_SEC = 120;

/**
 * @brief TICK_LENGTH 游戏刻长度（秒）
 */
const double TICK_LENGTH = 1.0 / TICKS_PER_SEC;

/**
 * @brief TICKS_PER_FRAME 每帧间游戏刻数目
 */
const int TICKS_PER_FRAME = 2;

/**
 * @brief EPSILON 用于比较浮点数，差绝对值小于此值则认为相等
 */
const double EPSILON = 1e-8;

/**
 * @brief GRAVITY 重力加速度
 */
const double GRAVITY = 30;

/**
 * @brief INTERSECT_DELTA 计算实体站立的阈值
 */
const double INTERSECT_DELTA = 0.05;

/**
 * @brief MAX_VELOCITY 速度分量的最大值
 */
const double MAX_VELOCITY = 25;

/**
 * @brief MAX_VELOCITY 加速度分量的最大值
 */
const double MAX_ACCELERATION = 20;

/**
 * @brief ENTITY_COLLISION_RANGE 实体碰撞计算更新范围
 */
const double ENTITY_COLLISION_RANGE = 2;

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
const double FALL_DAMAGE_SPEED = 12.0;

/**
 * @brief JUMP_SPEED 跳跃时纵向初速度大小
 */
const double JUMP_SPEED = 10.0;

/**
 * @brief WALK_SPEED 步行速度
 */
const double WALK_SPEED = 6.0;

/**
 * @brief EXPLOSION_DAMAGE_MULTIPLIER 爆炸伤害计算因子
 */
const double EXPLOSION_DAMAGE_MULTIPLIER = 3.0;

/**
 * @brief EXPLOSION_RADIUS_MULTIPLIER 爆炸半径因子
 */
const double EXPLOSION_RADIUS_MULTIPLIER = 1.0;

/**
 * @brief BOUNCE_BOTTOM_ATTENUATION 防止嵌入方块，撞到方块上方时的速度补偿因子
 */
const double BOUNCE_BOTTOM_ATTENUATION = 5e-3;

/**
 * @brief BOUNCE_SIDE_ATTENUATION 防止嵌入方块，撞到方块两侧时的速度补偿因子
 */
const double BOUNCE_SIDE_ATTENUATION = 0.5;


/**
 * @brief VOID_DAMAGE_PER_TICK 每个游戏刻的虚空伤害
 */
const double VOID_DAMAGE_PER_TICK = 0.2;

/**
 * @brief CAMERA_MOVE_TICKS 玩家在画面外时，运镜的总渲染刻数
 */
const double CAMERA_MOVE_TICKS = 1 * (TICKS_PER_SEC / TICKS_PER_FRAME);

/**
 * @brief SCREEN_EDGE_OUTER_WIDTH_MULTIPLIER 用于摄像机移动，玩家仍然在屏幕内时，开始摄像机移动的内边距
 */
const double SCREEN_EDGE_OUTER_WIDTH_MULTIPLIER = 0.2;

/**
 * @brief SCREEN_EDGE_OUTER_WIDTH_MULTIPLIER 用于摄像机移动，玩家仍然在屏幕内时，摄像机移动的终点内边距
 */
const double SCREEN_EDGE_INNER_WIDTH_MULTIPLIER = 0.45;
/**
 * @brief TNT_EXPLOSION_POWER TNT爆炸强度
 */
const double TNT_EXPLOSION_POWER = 14;
}

#endif // CONSTS_H
