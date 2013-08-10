/********************************************************************************
** Form generated from reading UI file 'ReinhardGlobalOperator.ui'
**
** Created: Sun 26. May 15:32:48 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REINHARDGLOBALOPERATOR_H
#define UI_REINHARDGLOBALOPERATOR_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReinhardGlobalOperator
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QSlider *keyValueSlider;
    QLabel *keyValueLabel;
    QLabel *keyValue;
    QGroupBox *burnOutGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *whiteValueLabel;
    QDoubleSpinBox *whiteValueSpinBox;
    QPushButton *whiteValueOkButton;

    void setupUi(QWidget *ReinhardGlobalOperator)
    {
        if (ReinhardGlobalOperator->objectName().isEmpty())
            ReinhardGlobalOperator->setObjectName(QString::fromUtf8("ReinhardGlobalOperator"));
        ReinhardGlobalOperator->resize(521, 455);
        ReinhardGlobalOperator->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ReinhardGlobalOperator);
#ifndef Q_OS_MAC
        verticalLayout->setContentsMargins(9, 9, 9, 9);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        keyValueSlider = new QSlider(ReinhardGlobalOperator);
        keyValueSlider->setObjectName(QString::fromUtf8("keyValueSlider"));
        keyValueSlider->setMinimum(1);
        keyValueSlider->setMaximum(99);
        keyValueSlider->setValue(18);
        keyValueSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(keyValueSlider, 0, 1, 1, 1);

        keyValueLabel = new QLabel(ReinhardGlobalOperator);
        keyValueLabel->setObjectName(QString::fromUtf8("keyValueLabel"));

        gridLayout->addWidget(keyValueLabel, 0, 0, 1, 1);

        keyValue = new QLabel(ReinhardGlobalOperator);
        keyValue->setObjectName(QString::fromUtf8("keyValue"));
        keyValue->setFrameShape(QFrame::StyledPanel);
        keyValue->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(keyValue, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        burnOutGroupBox = new QGroupBox(ReinhardGlobalOperator);
        burnOutGroupBox->setObjectName(QString::fromUtf8("burnOutGroupBox"));
        burnOutGroupBox->setCheckable(true);
        burnOutGroupBox->setChecked(false);
        gridLayout_2 = new QGridLayout(burnOutGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        whiteValueLabel = new QLabel(burnOutGroupBox);
        whiteValueLabel->setObjectName(QString::fromUtf8("whiteValueLabel"));

        gridLayout_2->addWidget(whiteValueLabel, 0, 0, 1, 1);

        whiteValueSpinBox = new QDoubleSpinBox(burnOutGroupBox);
        whiteValueSpinBox->setObjectName(QString::fromUtf8("whiteValueSpinBox"));
        whiteValueSpinBox->setMinimum(0.01);
        whiteValueSpinBox->setSingleStep(0.5);

        gridLayout_2->addWidget(whiteValueSpinBox, 0, 1, 1, 1);

        whiteValueOkButton = new QPushButton(burnOutGroupBox);
        whiteValueOkButton->setObjectName(QString::fromUtf8("whiteValueOkButton"));
        whiteValueOkButton->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(whiteValueOkButton, 0, 2, 1, 1);


        verticalLayout->addWidget(burnOutGroupBox);


        retranslateUi(ReinhardGlobalOperator);

        QMetaObject::connectSlotsByName(ReinhardGlobalOperator);
    } // setupUi

    void retranslateUi(QWidget *ReinhardGlobalOperator)
    {
        ReinhardGlobalOperator->setWindowTitle(QApplication::translate("ReinhardGlobalOperator", "ReinhardGlobalOperator", 0, QApplication::UnicodeUTF8));
        keyValueLabel->setText(QApplication::translate("ReinhardGlobalOperator", "Key Value:", 0, QApplication::UnicodeUTF8));
        keyValue->setText(QApplication::translate("ReinhardGlobalOperator", "0.18", 0, QApplication::UnicodeUTF8));
        burnOutGroupBox->setTitle(QApplication::translate("ReinhardGlobalOperator", "Burn Out", 0, QApplication::UnicodeUTF8));
        whiteValueLabel->setText(QApplication::translate("ReinhardGlobalOperator", "White Value:", 0, QApplication::UnicodeUTF8));
        whiteValueOkButton->setText(QApplication::translate("ReinhardGlobalOperator", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReinhardGlobalOperator: public Ui_ReinhardGlobalOperator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REINHARDGLOBALOPERATOR_H
