#ifndef EXPERIENCEHELPER_H
#define EXPERIENCEHELPER_H
#include <QObject>
#include <utility>

namespace parkour {
class ExperienceHelper : public QObject {
    Q_OBJECT
    int xpValue;

public:
    ExperienceHelper(int xpValue_);
    /**
    * @brief 获得等级信息
    * @returns [当前等级，当前等级中剩余的经验，升级到下一级差的经验]
    */
    std::tuple<int, int, int> toLevelInfo() const;
    int toXpValue() const;
    void setXpValue(int newValue);
};
}
#endif
