#ifndef ENTITY_H
#define ENTITY_H

#include "../utils/consts.h"
#include "../utils/direction.h"
#include "../utils/geometryhelper.h"
#include "boundingbox.h"
#include "icollidable.h"
#include <QObject>
#include <QString>
#include <QVector2D>

namespace parkour {
class Entity : public QObject, public ICollidable {
    Q_OBJECT

    QVector2D position;
    QVector2D velocity;
    QVector2D acceleration;

    double hp = 20.0;
    bool dying = false;

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
    virtual void collide(const ICollidable&, Direction) override {};
    /**
     * @brief 由子类重写实现特定的每tick更新
     */
    virtual void update() {};

    explicit Entity(QObject* parent = nullptr);
    virtual ~Entity() {};

    QVector2D getPosition() const;
    void setPosition(const QVector2D& value);

    QVector2D getVelocity() const override;
    void setVelocity(const QVector2D& value);

    QVector2D getAcceleration() const;
    void setAcceleration(const QVector2D& value);

    virtual double getHp() const final;
    virtual void setHp(double value) final;
    virtual void damage(double value);

    virtual QString getResourceLocation() = 0;

    bool getDying() const;

signals:
};
}

#endif // ENTITY_H
