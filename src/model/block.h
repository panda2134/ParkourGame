#ifndef BLOCK_H
#define BLOCK_H

#include "./boundingbox.h"
#include "./entity.h"
#include <QObject>

namespace parkour {

class Block : public QObject {
    Q_OBJECT
public:
    explicit Block() {};

    virtual QString getName() const = 0;
    virtual QString getResourceLocation() const = 0;
    virtual double getExplosionResistance() const = 0;
    /**
     * @brief getCurrentFrame 取得当前方块显示哪一帧
     * @param blockPos 方块位置
     * @return 返回值<0/超出范围，则每次屏幕刷新会切换到下一帧；否则显示指定帧（材质中从上往下排列，以0开始计数）
     */
    virtual int getCurrentFrame(const QPoint& blockPos);

    /**
     * @brief onExplosion 爆炸时的回调函数
     * @param blockPos 受到爆炸的方块位置
     * @param power 此位置的爆炸强度
     */
    virtual void onExplosion(QPoint blockPos, double power);
	virtual void onStand(QPoint blockPos, Entity& entity);
    virtual void collide(QPoint, Entity&, Direction) {};
    virtual BoundingBox getBoundingBox() const;
signals:
};

}

#endif // BLOCK_H
