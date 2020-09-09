/********************************************************************************
** Form generated from reading UI file 'saveselection.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVESELECTION_H
#define UI_SAVESELECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "view/widgets/styledpushbutton.h"

QT_BEGIN_NAMESPACE

class Ui_SaveSelection
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QListWidget *listWidget;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    StyledPushButton *createButton;
    StyledPushButton *deleteButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *SaveSelection)
    {
        if (SaveSelection->objectName().isEmpty())
            SaveSelection->setObjectName(QString::fromUtf8("SaveSelection"));
        SaveSelection->setWindowModality(Qt::WindowModal);
        SaveSelection->resize(640, 320);
        SaveSelection->setMinimumSize(QSize(640, 320));
        SaveSelection->setStyleSheet(QString::fromUtf8("#SaveSelection{background: url(:/assets/gui/options_background.png);}\n"
"QInputDialog {\n"
"background-color: white !important;\n"
"}"));
        verticalLayout = new QVBoxLayout(SaveSelection);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(SaveSelection);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color:white;"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_2);

        listWidget = new QListWidget(SaveSelection);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setStyleSheet(QString::fromUtf8("#listWidget{background:transparent;}"));

        verticalLayout->addWidget(listWidget);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        createButton = new StyledPushButton(SaveSelection);
        createButton->setObjectName(QString::fromUtf8("createButton"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(createButton->sizePolicy().hasHeightForWidth());
        createButton->setSizePolicy(sizePolicy);
        createButton->setStyleSheet(QString::fromUtf8("color:white;"));

        horizontalLayout->addWidget(createButton);

        deleteButton = new StyledPushButton(SaveSelection);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setEnabled(false);
        sizePolicy.setHeightForWidth(deleteButton->sizePolicy().hasHeightForWidth());
        deleteButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(deleteButton);

        horizontalSpacer_2 = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(SaveSelection);

        QMetaObject::connectSlotsByName(SaveSelection);
    } // setupUi

    void retranslateUi(QWidget *SaveSelection)
    {
        SaveSelection->setWindowTitle(QCoreApplication::translate("SaveSelection", "\351\200\211\346\213\251\345\255\230\346\241\243...", nullptr));
        label->setText(QCoreApplication::translate("SaveSelection", "\351\200\211\346\213\251\345\255\230\346\241\243...", nullptr));
        createButton->setText(QCoreApplication::translate("SaveSelection", "\345\210\233\345\273\272\346\226\260\345\255\230\346\241\243", nullptr));
        deleteButton->setText(QCoreApplication::translate("SaveSelection", "\345\210\240\351\231\244\345\255\230\346\241\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveSelection: public Ui_SaveSelection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVESELECTION_H
