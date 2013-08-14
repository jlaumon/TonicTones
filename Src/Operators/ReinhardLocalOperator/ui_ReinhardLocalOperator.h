/********************************************************************************
** Form generated from reading UI file 'ReinhardLocalOperator.ui'
**
** Created: Tue 13. Aug 20:02:07 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REINHARDLOCALOPERATOR_H
#define UI_REINHARDLOCALOPERATOR_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReinhardLocalOperator
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QSlider *keyValueSlider;
    QLabel *keyValueLabel;
    QLabel *keyValue;
    QLabel *sharpeningLabel;
    QSlider *sharpeningSlider;
    QLabel *sharpening;

    void setupUi(QWidget *ReinhardLocalOperator)
    {
        if (ReinhardLocalOperator->objectName().isEmpty())
            ReinhardLocalOperator->setObjectName(QString::fromUtf8("ReinhardLocalOperator"));
        ReinhardLocalOperator->resize(640, 480);
        ReinhardLocalOperator->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ReinhardLocalOperator);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        keyValueSlider = new QSlider(ReinhardLocalOperator);
        keyValueSlider->setObjectName(QString::fromUtf8("keyValueSlider"));
        keyValueSlider->setMinimum(1);
        keyValueSlider->setMaximum(99);
        keyValueSlider->setValue(18);
        keyValueSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(keyValueSlider, 0, 1, 1, 1);

        keyValueLabel = new QLabel(ReinhardLocalOperator);
        keyValueLabel->setObjectName(QString::fromUtf8("keyValueLabel"));

        gridLayout->addWidget(keyValueLabel, 0, 0, 1, 1);

        keyValue = new QLabel(ReinhardLocalOperator);
        keyValue->setObjectName(QString::fromUtf8("keyValue"));
        keyValue->setFrameShape(QFrame::StyledPanel);
        keyValue->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(keyValue, 0, 2, 1, 1);

        sharpeningLabel = new QLabel(ReinhardLocalOperator);
        sharpeningLabel->setObjectName(QString::fromUtf8("sharpeningLabel"));

        gridLayout->addWidget(sharpeningLabel, 1, 0, 1, 1);

        sharpeningSlider = new QSlider(ReinhardLocalOperator);
        sharpeningSlider->setObjectName(QString::fromUtf8("sharpeningSlider"));
        sharpeningSlider->setMinimum(1);
        sharpeningSlider->setMaximum(20);
        sharpeningSlider->setValue(8);
        sharpeningSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sharpeningSlider, 1, 1, 1, 1);

        sharpening = new QLabel(ReinhardLocalOperator);
        sharpening->setObjectName(QString::fromUtf8("sharpening"));
        sharpening->setFrameShape(QFrame::StyledPanel);
        sharpening->setFrameShadow(QFrame::Sunken);
        sharpening->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(sharpening, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(ReinhardLocalOperator);

        QMetaObject::connectSlotsByName(ReinhardLocalOperator);
    } // setupUi

    void retranslateUi(QWidget *ReinhardLocalOperator)
    {
        ReinhardLocalOperator->setWindowTitle(QApplication::translate("ReinhardLocalOperator", "ReinhardLocalOperator", 0, QApplication::UnicodeUTF8));
        keyValueLabel->setText(QApplication::translate("ReinhardLocalOperator", "Key Value:", 0, QApplication::UnicodeUTF8));
        keyValue->setText(QApplication::translate("ReinhardLocalOperator", "0.18", 0, QApplication::UnicodeUTF8));
        sharpeningLabel->setText(QApplication::translate("ReinhardLocalOperator", "Sharpening:", 0, QApplication::UnicodeUTF8));
        sharpening->setText(QApplication::translate("ReinhardLocalOperator", "8", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReinhardLocalOperator: public Ui_ReinhardLocalOperator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REINHARDLOCALOPERATOR_H
