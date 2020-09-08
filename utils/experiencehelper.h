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
		std::tuple<int, int, int> toLevel() const;
		int toXpValue() const;
		void setXpValue(int newValue);
	};
}
#endif

