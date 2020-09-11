/********************************************************************************
** Form generated from reading UI file 'saveitem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEITEM_H
#define UI_SAVEITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SaveItem {
public:
    QHBoxLayout* horizontalLayout;
    QLabel* imgLabel;
    QSpacerItem* horizontalSpacer;
    QVBoxLayout* verticalLayout;
    QLabel* saveNameLabel;
    QLabel* lastModifiedLabel;

    void setupUi(QWidget* SaveItem) {
        if (SaveItem->objectName().isEmpty())
            SaveItem->setObjectName(QString::fromUtf8("SaveItem"));
        SaveItem->resize(563, 108);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SaveItem->sizePolicy().hasHeightForWidth());
        SaveItem->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(SaveItem);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        imgLabel = new QLabel(SaveItem);
        imgLabel->setObjectName(QString::fromUtf8("imgLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(imgLabel->sizePolicy().hasHeightForWidth());
        imgLabel->setSizePolicy(sizePolicy1);
        imgLabel->setMinimumSize(QSize(160, 90));
        imgLabel->setMaximumSize(QSize(160, 90));

        horizontalLayout->addWidget(imgLabel);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        saveNameLabel = new QLabel(SaveItem);
        saveNameLabel->setObjectName(QString::fromUtf8("saveNameLabel"));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setBold(true);
        font.setWeight(62);
        saveNameLabel->setFont(font);
        saveNameLabel->setStyleSheet(QString::fromUtf8("color:white;\n"
                                                       "font-size: 25px;\n"
                                                       "font-weight: 500;"));
        saveNameLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

        verticalLayout->addWidget(saveNameLabel);

        lastModifiedLabel = new QLabel(SaveItem);
        lastModifiedLabel->setObjectName(QString::fromUtf8("lastModifiedLabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font1.setPointSize(12);
        lastModifiedLabel->setFont(font1);
        lastModifiedLabel->setStyleSheet(QString::fromUtf8("color: #cccccc;\n"
                                                           "font-size:0.6em;"));
        lastModifiedLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

        verticalLayout->addWidget(lastModifiedLabel);

        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 9);

        retranslateUi(SaveItem);

        QMetaObject::connectSlotsByName(SaveItem);
    } // setupUi

    void retranslateUi(QWidget* SaveItem) {
        SaveItem->setWindowTitle(QCoreApplication::translate("SaveItem", "SaveItem", nullptr));
        imgLabel->setText(QString());
        saveNameLabel->setText(QCoreApplication::translate("SaveItem", "TextLabel", nullptr));
        lastModifiedLabel->setText(QCoreApplication::translate("SaveItem", "TextLabel", nullptr));
    } // retranslateUi
};

namespace Ui {
class SaveItem : public Ui_SaveItem { };
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEITEM_H
