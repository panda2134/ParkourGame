#ifndef ENTITY_H
#define ENTITY_H

#include "../utils/direction.h"
#include "boundingbox.h"
#include "icollidable.h"
#include <QObject>
#include <QString>
#include <QVector2D>

namespace parkour {

class Entity : public QObject, public ICollidable {
	Q_OBJECT

	double ENTITY_MIN_VELOCITY = 0.1;

    QVector2D position;
    QVector2D velocity;
    QVector2D acceleration;

    double hp = 20.0;
    bool dying = false;
    bool onFloor = true;

public:
    virtual QString getName() const = 0;
    /**
     * @brief 每个tick调用一次，用于更新位置；此后需要在collide中设置状态
     */
	void updatePosition();
    /**
     * @brief 实体碰撞处理器，子类可以重写以实现特定的碰撞处理
     * @param dir 自己被撞到的方向
     */
    virtual void collide(ICollidable&, Direction) override {};
    /**
     * @brief 由子类重写实现特定的每tick更新
     */
    virtual void update() {};
    /**
     * @brief 是否显示死亡动画
    */
    virtual bool showDeathAnimation() const;
    /**
     * @brief 是否受到重力
    */
    virtual bool isAffectedByGravity() const;
    /**
     * @brief 速度是否受到爆炸冲击波影响
    */
    virtual bool isAffectedByExplosionWave() const;

    /**
     *  @brief 设置实体Bounding Box左下角
     *  @param bottomLeft 左下角的位置
     */
    void placeBoundingBoxAt(const QVector2D& bottomLeft);
    /**
     * @brief getBottomLeft 取得Bounding Box左下角位置
     */
    QVector2D getBoundingBoxBottomLeft();

    explicit Entity(QObject* parent = nullptr);
    virtual ~Entity() {};

    QVector2D getPosition() const;
    void setPosition(const QVector2D& value);

    QVector2D getVelocity() const override;
    void setVelocity(const QVector2D& value) override;

    QVector2D getAcceleration() const;
    void setAcceleration(const QVector2D& value);

    double getHp() const;
    void setHp(double value);
	/**
	 * @brief 血量回满
	*/
	void regenerate();
    virtual void damage(double value);
	virtual double getMaxHp() const;

    virtual QString getResourceLocation() = 0;
    /**
     * @brief getTextureDimensions 取得材质渲染的大小
     * @return 游戏坐标下，材质渲染的大小
     */
    virtual QVector2D getTextureDimensions() = 0;

    bool isDying() const;

    bool isOnFloor() const;
    void setOnFloor(bool value);

    BoundingBoxWorld getBoundingBoxWorld() const override final;
    virtual BoundingBox getBoundingBox() const = 0;
};
}

#endif // ENTITY_H
