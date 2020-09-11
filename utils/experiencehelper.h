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
    * @brief ��õȼ���Ϣ
    * @returns [��ǰ�ȼ�����ǰ�ȼ���ʣ��ľ��飬��������һ����ľ���]
    */
    std::tuple<int, int, int> toLevelInfo() const;
    int toXpValue() const;
    void setXpValue(int newValue);
};
}
#endif
