/********************************************************************************
** Form generated from reading UI file 'ScreenColorsDialog.ui'
**
** Created: Tue 13. Aug 20:06:34 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENCOLORSDIALOG_H
#define UI_SCREENCOLORSDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ScreenColorsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QDoubleSpinBox *m00;
    QDoubleSpinBox *m10;
    QDoubleSpinBox *m20;
    QDoubleSpinBox *m11;
    QDoubleSpinBox *m12;
    QDoubleSpinBox *m01;
    QDoubleSpinBox *m21;
    QDoubleSpinBox *m02;
    QDoubleSpinBox *m22;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ScreenColorsDialog)
    {
        if (ScreenColorsDialog->objectName().isEmpty())
            ScreenColorsDialog->setObjectName(QString::fromUtf8("ScreenColorsDialog"));
        ScreenColorsDialog->resize(385, 142);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenColorsDialog->sizePolicy().hasHeightForWidth());
        ScreenColorsDialog->setSizePolicy(sizePolicy);
        ScreenColorsDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ScreenColorsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m00 = new QDoubleSpinBox(ScreenColorsDialog);
        m00->setObjectName(QString::fromUtf8("m00"));
        m00->setDecimals(5);
        m00->setMinimum(-100);
        m00->setMaximum(100);
        m00->setSingleStep(0.01);

        gridLayout->addWidget(m00, 0, 1, 1, 1);

        m10 = new QDoubleSpinBox(ScreenColorsDialog);
        m10->setObjectName(QString::fromUtf8("m10"));
        m10->setDecimals(5);
        m10->setMinimum(-100);
        m10->setSingleStep(0.01);

        gridLayout->addWidget(m10, 1, 1, 1, 1);

        m20 = new QDoubleSpinBox(ScreenColorsDialog);
        m20->setObjectName(QString::fromUtf8("m20"));
        m20->setDecimals(5);
        m20->setMinimum(-100);
        m20->setSingleStep(0.01);

        gridLayout->addWidget(m20, 2, 1, 1, 1);

        m11 = new QDoubleSpinBox(ScreenColorsDialog);
        m11->setObjectName(QString::fromUtf8("m11"));
        m11->setDecimals(5);
        m11->setMinimum(-100);
        m11->setSingleStep(0.01);

        gridLayout->addWidget(m11, 1, 2, 1, 1);

        m12 = new QDoubleSpinBox(ScreenColorsDialog);
        m12->setObjectName(QString::fromUtf8("m12"));
        m12->setDecimals(5);
        m12->setMinimum(-100);
        m12->setSingleStep(0.01);

        gridLayout->addWidget(m12, 1, 3, 1, 1);

        m01 = new QDoubleSpinBox(ScreenColorsDialog);
        m01->setObjectName(QString::fromUtf8("m01"));
        m01->setDecimals(5);
        m01->setMinimum(-100);
        m01->setMaximum(100);
        m01->setSingleStep(0.01);

        gridLayout->addWidget(m01, 0, 2, 1, 1);

        m21 = new QDoubleSpinBox(ScreenColorsDialog);
        m21->setObjectName(QString::fromUtf8("m21"));
        m21->setDecimals(5);
        m21->setMinimum(-100);
        m21->setSingleStep(0.01);

        gridLayout->addWidget(m21, 2, 2, 1, 1);

        m02 = new QDoubleSpinBox(ScreenColorsDialog);
        m02->setObjectName(QString::fromUtf8("m02"));
        m02->setDecimals(5);
        m02->setMinimum(-100);
        m02->setMaximum(100);
        m02->setSingleStep(0.01);

        gridLayout->addWidget(m02, 0, 3, 1, 1);

        m22 = new QDoubleSpinBox(ScreenColorsDialog);
        m22->setObjectName(QString::fromUtf8("m22"));
        m22->setDecimals(5);
        m22->setMinimum(-100);
        m22->setSingleStep(0.01);

        gridLayout->addWidget(m22, 2, 3, 1, 1);

        label = new QLabel(ScreenColorsDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(ScreenColorsDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(ScreenColorsDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        gridLayout->setColumnStretch(1, 1);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 1);

        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(ScreenColorsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        sizePolicy.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ScreenColorsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ScreenColorsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ScreenColorsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ScreenColorsDialog);
    } // setupUi

    void retranslateUi(QDialog *ScreenColorsDialog)
    {
        ScreenColorsDialog->setWindowTitle(QApplication::translate("ScreenColorsDialog", "Screen Colors XYZ Coordinates", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ScreenColorsDialog", "R:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ScreenColorsDialog", "G:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ScreenColorsDialog", "B:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ScreenColorsDialog: public Ui_ScreenColorsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENCOLORSDIALOG_H
