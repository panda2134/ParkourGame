#include "experiencehelper.h"

namespace parkour {
ExperienceHelper::ExperienceHelper(int xpValue_)
    : xpValue(xpValue_) {
}
std::tuple<int, int, int> ExperienceHelper::toLevelInfo() const {
    int level = 0, xpLeft = xpValue;
    for (; level <= 15; ++level) {
        int xpRequired = 2 * level + 7;
        if (xpLeft >= xpRequired) {
            xpLeft -= xpRequired;
        } else {
            return std::make_tuple(level, xpLeft, xpRequired);
        }
    }
    for (; level <= 30; ++level) {
        int xpRequired = 5 * level - 38;
        if (xpLeft >= xpRequired) {
            xpLeft -= xpRequired;
        } else {
            return std::make_tuple(level, xpLeft, xpRequired);
        }
    }
    for (;; ++level) {
        int xpRequired = 9 * level - 158;
        if (xpLeft >= xpRequired) {
            xpLeft -= xpRequired;
        } else {
            return std::make_tuple(level, xpLeft, xpRequired);
        }
    }
}
int ExperienceHelper::toXpValue() const {
    return xpValue;
}
void ExperienceHelper::setXpValue(int newValue) {
    xpValue = newValue;
}
}