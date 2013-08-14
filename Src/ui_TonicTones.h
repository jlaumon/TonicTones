/********************************************************************************
** Form generated from reading UI file 'TonicTones.ui'
**
** Created: Tue 13. Aug 20:06:34 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TONICTONES_H
#define UI_TONICTONES_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TonicTones
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionScreenColors;
    QWidget *centralwidget;
    QHBoxLayout *centralwidgetLayout;
    QWidget *displayArea;
    QVBoxLayout *displayAreaLayout;
    QWidget *optionsWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *operatorGroupBox;
    QVBoxLayout *operatorGroupBoxLayout;
    QComboBox *operatorComboBox;
    QGroupBox *operatorOptionsGroupBox;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *generalOptions;
    QGridLayout *gridLayout_3;
    QGroupBox *gammaGroupBox;
    QGridLayout *gridLayout;
    QSlider *gammaSlider;
    QLabel *gammaValue;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSettings;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TonicTones)
    {
        if (TonicTones->objectName().isEmpty())
            TonicTones->setObjectName(QString::fromUtf8("TonicTones"));
        TonicTones->resize(839, 534);
        TonicTones->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionOpen = new QAction(TonicTones);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(TonicTones);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionExit->setShortcutContext(Qt::WindowShortcut);
        actionScreenColors = new QAction(TonicTones);
        actionScreenColors->setObjectName(QString::fromUtf8("actionScreenColors"));
        centralwidget = new QWidget(TonicTones);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidgetLayout = new QHBoxLayout(centralwidget);
        centralwidgetLayout->setObjectName(QString::fromUtf8("centralwidgetLayout"));
        displayArea = new QWidget(centralwidget);
        displayArea->setObjectName(QString::fromUtf8("displayArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(displayArea->sizePolicy().hasHeightForWidth());
        displayArea->setSizePolicy(sizePolicy);
        displayAreaLayout = new QVBoxLayout(displayArea);
        displayAreaLayout->setContentsMargins(0, 0, 0, 0);
        displayAreaLayout->setObjectName(QString::fromUtf8("displayAreaLayout"));

        centralwidgetLayout->addWidget(displayArea);

        optionsWidget = new QWidget(centralwidget);
        optionsWidget->setObjectName(QString::fromUtf8("optionsWidget"));
        optionsWidget->setMinimumSize(QSize(320, 0));
        optionsWidget->setMaximumSize(QSize(320, 16777215));
        verticalLayout = new QVBoxLayout(optionsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        operatorGroupBox = new QGroupBox(optionsWidget);
        operatorGroupBox->setObjectName(QString::fromUtf8("operatorGroupBox"));
        operatorGroupBox->setMinimumSize(QSize(0, 0));
        operatorGroupBox->setCheckable(true);
        operatorGroupBoxLayout = new QVBoxLayout(operatorGroupBox);
        operatorGroupBoxLayout->setObjectName(QString::fromUtf8("operatorGroupBoxLayout"));
        operatorComboBox = new QComboBox(operatorGroupBox);
        operatorComboBox->setObjectName(QString::fromUtf8("operatorComboBox"));

        operatorGroupBoxLayout->addWidget(operatorComboBox);


        verticalLayout->addWidget(operatorGroupBox);

        operatorOptionsGroupBox = new QGroupBox(optionsWidget);
        operatorOptionsGroupBox->setObjectName(QString::fromUtf8("operatorOptionsGroupBox"));
        operatorOptionsGroupBox->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(operatorOptionsGroupBox);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        verticalLayout->addWidget(operatorOptionsGroupBox);

        generalOptions = new QGroupBox(optionsWidget);
        generalOptions->setObjectName(QString::fromUtf8("generalOptions"));
        gridLayout_3 = new QGridLayout(generalOptions);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gammaGroupBox = new QGroupBox(generalOptions);
        gammaGroupBox->setObjectName(QString::fromUtf8("gammaGroupBox"));
        gammaGroupBox->setCheckable(true);
        gridLayout = new QGridLayout(gammaGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gammaSlider = new QSlider(gammaGroupBox);
        gammaSlider->setObjectName(QString::fromUtf8("gammaSlider"));
        gammaSlider->setMinimum(10);
        gammaSlider->setMaximum(300);
        gammaSlider->setSingleStep(1);
        gammaSlider->setPageStep(10);
        gammaSlider->setValue(100);
        gammaSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(gammaSlider, 0, 0, 1, 1);

        gammaValue = new QLabel(gammaGroupBox);
        gammaValue->setObjectName(QString::fromUtf8("gammaValue"));
        gammaValue->setFrameShape(QFrame::StyledPanel);
        gammaValue->setFrameShadow(QFrame::Sunken);
        gammaValue->setLineWidth(1);
        gammaValue->setMidLineWidth(0);
        gammaValue->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        gammaValue->setWordWrap(false);

        gridLayout->addWidget(gammaValue, 0, 1, 1, 1);


        gridLayout_3->addWidget(gammaGroupBox, 0, 0, 1, 1);


        verticalLayout->addWidget(generalOptions);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        centralwidgetLayout->addWidget(optionsWidget);

        TonicTones->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TonicTones);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 839, 27));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        TonicTones->setMenuBar(menubar);
        statusbar = new QStatusBar(TonicTones);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QPalette palette;
        QBrush brush(QColor(16, 16, 16, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(230, 221, 213, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        QBrush brush2(QColor(26, 26, 26, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        statusbar->setPalette(palette);
        TonicTones->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuSettings->addAction(actionScreenColors);

        retranslateUi(TonicTones);
        QObject::connect(actionExit, SIGNAL(triggered()), TonicTones, SLOT(close()));

        QMetaObject::connectSlotsByName(TonicTones);
    } // setupUi

    void retranslateUi(QMainWindow *TonicTones)
    {
        TonicTones->setWindowTitle(QApplication::translate("TonicTones", "TonicTones", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("TonicTones", "Open...", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("TonicTones", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("TonicTones", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("TonicTones", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionScreenColors->setText(QApplication::translate("TonicTones", "Set screen colors XYZ coordinates...", 0, QApplication::UnicodeUTF8));
        operatorGroupBox->setTitle(QApplication::translate("TonicTones", "Operator", 0, QApplication::UnicodeUTF8));
        operatorOptionsGroupBox->setTitle(QApplication::translate("TonicTones", "Operator Options", 0, QApplication::UnicodeUTF8));
        generalOptions->setTitle(QApplication::translate("TonicTones", "General Options", 0, QApplication::UnicodeUTF8));
        gammaGroupBox->setTitle(QApplication::translate("TonicTones", "Gamma Correction", 0, QApplication::UnicodeUTF8));
        gammaValue->setText(QApplication::translate("TonicTones", "1.00", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("TonicTones", "File", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("TonicTones", "Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TonicTones: public Ui_TonicTones {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TONICTONES_H
