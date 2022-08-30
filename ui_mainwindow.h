/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *placeHolder;
    QWidget *widget;
    QWidget *widget1;
    QGridLayout *gridLayout_6;
    QLabel *label_12;
    QLCDNumber *CD_Val;
    QLCDNumber *Mass_Val;
    QLCDNumber *Inertia_Val;
    QLabel *label_33;
    QLabel *label_34;
    QWidget *widget2;
    QGridLayout *gridLayout_2;
    QLCDNumber *ForZ0;
    QLCDNumber *ForY0_2;
    QLabel *label_21;
    QLabel *label_52;
    QLabel *label_50;
    QLabel *label_57;
    QLCDNumber *ForY0;
    QLabel *label_20;
    QLCDNumber *DesX1;
    QLCDNumber *ForX0;
    QLabel *label_67;
    QLCDNumber *ForZ1_2;
    QLCDNumber *DesZ0;
    QLCDNumber *ForZ0_2;
    QLCDNumber *DesY1;
    QLCDNumber *ForZ1;
    QLCDNumber *ForY1_2;
    QLabel *label_69;
    QLabel *label_19;
    QLCDNumber *DesY0;
    QLCDNumber *ForY1;
    QLCDNumber *DesZ1;
    QLCDNumber *ForX0_2;
    QLabel *label_53;
    QLCDNumber *DesX0;
    QLabel *label_49;
    QLCDNumber *ForX1_2;
    QLabel *label_58;
    QLCDNumber *ForX1;
    QLabel *label_64;
    QTextEdit *fakeWorldObj;
    QWidget *widget3;
    QGridLayout *gridLayout;
    QPushButton *StiffnessExp;
    QPushButton *HoxelExp;
    QPushButton *Manual;
    QPushButton *StiffnMassCombined;
    QPushButton *FingerMappingExp;
    QPushButton *dynamicEnvironment;
    QWidget *widget4;
    QGridLayout *gridLayout_3;
    QLabel *label_31;
    QLabel *label_22;
    QLCDNumber *lcdNumberGraphicsRate;
    QLCDNumber *lcdNumberHapticRate;
    QWidget *widget5;
    QGridLayout *gridLayout_8;
    QLabel *label_23;
    QLabel *label_51;
    QLCDNumber *trialNo;
    QLCDNumber *pairNo;
    QLabel *label_24;
    QSpinBox *subject_no;
    QCheckBox *AdjustTrialNo;
    QSpinBox *SetTrialNo;
    QLabel *label_56;
    QWidget *widget6;
    QGridLayout *gridLayout_7;
    QLabel *label;
    QLabel *label_26;
    QLCDNumber *MotorLCD_1;
    QLCDNumber *condition;
    QLCDNumber *StrokeLCD1_0;
    QLabel *label_37;
    QLabel *label_36;
    QLCDNumber *StrokeLCD1_1;
    QLabel *label_59;
    QLabel *label_27;
    QLCDNumber *MotorLCD_0;
    QRadioButton *Condition1;
    QLCDNumber *direction;
    QWidget *widget7;
    QGridLayout *gridLayout_4;
    QLabel *label_14;
    QLabel *label_16;
    QLabel *label_17;
    QLCDNumber *mass1_show;
    QLCDNumber *stiff1Scale_show;
    QLabel *label_15;
    QLabel *label_25;
    QLabel *label_18;
    QLCDNumber *mass2_show;
    QLCDNumber *stiff2Scale_show;
    QWidget *widget8;
    QGridLayout *gridLayout_5;
    QLabel *label_35;
    QLineEdit *mass_set;
    QLabel *label_32;
    QLineEdit *cd_ratio;
    QLabel *label_11;
    QLineEdit *inertiaValue;
    QRadioButton *VRControl;
    QRadioButton *equalBox;
    QCheckBox *equalBoxTouch;
    QWidget *widget9;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *text;
    QTextBrowser *mappingTextBox;
    QWidget *widget10;
    QGridLayout *gridLayout_10;
    QPushButton *turnOff;
    QCheckBox *JakeRenderCheckBox;
    QRadioButton *reverseBox;
    QCheckBox *CD_Vertical_Box;
    QWidget *widget11;
    QGridLayout *gridLayout_9;
    QLabel *label_54;
    QLCDNumber *Act1_show;
    QSlider *Act1;
    QLabel *label_55;
    QLCDNumber *Act2_show;
    QSlider *Act2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(2300, 796);
        MainWindow->setMinimumSize(QSize(0, 0));
        QPalette palette;
        MainWindow->setPalette(palette);
        QFont font;
        font.setPointSize(12);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral("D:/Pictures/Icon.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAutoFillBackground(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        centralWidget->setMinimumSize(QSize(2300, 743));
        placeHolder = new QWidget(centralWidget);
        placeHolder->setObjectName(QStringLiteral("placeHolder"));
        placeHolder->setGeometry(QRect(0, 0, 100, 30));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(1700, 530, 261, 81));
        widget->setAutoFillBackground(false);
        widget1 = new QWidget(widget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(10, 5, 241, 81));
        gridLayout_6 = new QGridLayout(widget1);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(widget1);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_12, 0, 2, 1, 1);

        CD_Val = new QLCDNumber(widget1);
        CD_Val->setObjectName(QStringLiteral("CD_Val"));
        CD_Val->setDigitCount(3);

        gridLayout_6->addWidget(CD_Val, 1, 1, 1, 1);

        Mass_Val = new QLCDNumber(widget1);
        Mass_Val->setObjectName(QStringLiteral("Mass_Val"));
        Mass_Val->setDigitCount(5);

        gridLayout_6->addWidget(Mass_Val, 1, 2, 1, 1);

        Inertia_Val = new QLCDNumber(widget1);
        Inertia_Val->setObjectName(QStringLiteral("Inertia_Val"));
        Inertia_Val->setDigitCount(3);

        gridLayout_6->addWidget(Inertia_Val, 1, 0, 1, 1);

        label_33 = new QLabel(widget1);
        label_33->setObjectName(QStringLiteral("label_33"));
        label_33->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_33, 0, 1, 1, 1);

        label_34 = new QLabel(widget1);
        label_34->setObjectName(QStringLiteral("label_34"));
        label_34->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_34, 0, 0, 1, 1);

        widget2 = new QWidget(centralWidget);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setGeometry(QRect(1140, 30, 461, 211));
        gridLayout_2 = new QGridLayout(widget2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        ForZ0 = new QLCDNumber(widget2);
        ForZ0->setObjectName(QStringLiteral("ForZ0"));
        ForZ0->setDigitCount(6);

        gridLayout_2->addWidget(ForZ0, 2, 2, 1, 1);

        ForY0_2 = new QLCDNumber(widget2);
        ForY0_2->setObjectName(QStringLiteral("ForY0_2"));
        ForY0_2->setDigitCount(6);

        gridLayout_2->addWidget(ForY0_2, 4, 1, 1, 1);

        label_21 = new QLabel(widget2);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_2->addWidget(label_21, 5, 2, 1, 1);

        label_52 = new QLabel(widget2);
        label_52->setObjectName(QStringLiteral("label_52"));

        gridLayout_2->addWidget(label_52, 1, 0, 1, 3);

        label_50 = new QLabel(widget2);
        label_50->setObjectName(QStringLiteral("label_50"));
        label_50->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_50, 0, 3, 1, 3);

        label_57 = new QLabel(widget2);
        label_57->setObjectName(QStringLiteral("label_57"));

        gridLayout_2->addWidget(label_57, 1, 3, 1, 3);

        ForY0 = new QLCDNumber(widget2);
        ForY0->setObjectName(QStringLiteral("ForY0"));
        ForY0->setDigitCount(6);

        gridLayout_2->addWidget(ForY0, 2, 1, 1, 1);

        label_20 = new QLabel(widget2);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_2->addWidget(label_20, 5, 1, 1, 1);

        DesX1 = new QLCDNumber(widget2);
        DesX1->setObjectName(QStringLiteral("DesX1"));
        DesX1->setDigitCount(6);

        gridLayout_2->addWidget(DesX1, 6, 3, 1, 1);

        ForX0 = new QLCDNumber(widget2);
        ForX0->setObjectName(QStringLiteral("ForX0"));
        ForX0->setDigitCount(6);

        gridLayout_2->addWidget(ForX0, 2, 0, 1, 1);

        label_67 = new QLabel(widget2);
        label_67->setObjectName(QStringLiteral("label_67"));

        gridLayout_2->addWidget(label_67, 5, 4, 1, 1);

        ForZ1_2 = new QLCDNumber(widget2);
        ForZ1_2->setObjectName(QStringLiteral("ForZ1_2"));
        ForZ1_2->setDigitCount(6);

        gridLayout_2->addWidget(ForZ1_2, 4, 5, 1, 1);

        DesZ0 = new QLCDNumber(widget2);
        DesZ0->setObjectName(QStringLiteral("DesZ0"));
        DesZ0->setDigitCount(6);

        gridLayout_2->addWidget(DesZ0, 6, 2, 1, 1);

        ForZ0_2 = new QLCDNumber(widget2);
        ForZ0_2->setObjectName(QStringLiteral("ForZ0_2"));
        ForZ0_2->setDigitCount(6);

        gridLayout_2->addWidget(ForZ0_2, 4, 2, 1, 1);

        DesY1 = new QLCDNumber(widget2);
        DesY1->setObjectName(QStringLiteral("DesY1"));
        DesY1->setDigitCount(6);

        gridLayout_2->addWidget(DesY1, 6, 4, 1, 1);

        ForZ1 = new QLCDNumber(widget2);
        ForZ1->setObjectName(QStringLiteral("ForZ1"));
        ForZ1->setDigitCount(6);

        gridLayout_2->addWidget(ForZ1, 2, 5, 1, 1);

        ForY1_2 = new QLCDNumber(widget2);
        ForY1_2->setObjectName(QStringLiteral("ForY1_2"));
        ForY1_2->setDigitCount(6);

        gridLayout_2->addWidget(ForY1_2, 4, 4, 1, 1);

        label_69 = new QLabel(widget2);
        label_69->setObjectName(QStringLiteral("label_69"));

        gridLayout_2->addWidget(label_69, 5, 5, 1, 1);

        label_19 = new QLabel(widget2);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_2->addWidget(label_19, 5, 0, 1, 1);

        DesY0 = new QLCDNumber(widget2);
        DesY0->setObjectName(QStringLiteral("DesY0"));
        DesY0->setDigitCount(6);

        gridLayout_2->addWidget(DesY0, 6, 1, 1, 1);

        ForY1 = new QLCDNumber(widget2);
        ForY1->setObjectName(QStringLiteral("ForY1"));
        ForY1->setDigitCount(6);

        gridLayout_2->addWidget(ForY1, 2, 4, 1, 1);

        DesZ1 = new QLCDNumber(widget2);
        DesZ1->setObjectName(QStringLiteral("DesZ1"));
        DesZ1->setDigitCount(6);

        gridLayout_2->addWidget(DesZ1, 6, 5, 1, 1);

        ForX0_2 = new QLCDNumber(widget2);
        ForX0_2->setObjectName(QStringLiteral("ForX0_2"));
        ForX0_2->setDigitCount(6);

        gridLayout_2->addWidget(ForX0_2, 4, 0, 1, 1);

        label_53 = new QLabel(widget2);
        label_53->setObjectName(QStringLiteral("label_53"));

        gridLayout_2->addWidget(label_53, 3, 0, 1, 3);

        DesX0 = new QLCDNumber(widget2);
        DesX0->setObjectName(QStringLiteral("DesX0"));
        DesX0->setDigitCount(6);

        gridLayout_2->addWidget(DesX0, 6, 0, 1, 1);

        label_49 = new QLabel(widget2);
        label_49->setObjectName(QStringLiteral("label_49"));
        label_49->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_49, 0, 0, 1, 3);

        ForX1_2 = new QLCDNumber(widget2);
        ForX1_2->setObjectName(QStringLiteral("ForX1_2"));
        ForX1_2->setDigitCount(6);

        gridLayout_2->addWidget(ForX1_2, 4, 3, 1, 1);

        label_58 = new QLabel(widget2);
        label_58->setObjectName(QStringLiteral("label_58"));

        gridLayout_2->addWidget(label_58, 3, 3, 1, 3);

        ForX1 = new QLCDNumber(widget2);
        ForX1->setObjectName(QStringLiteral("ForX1"));
        ForX1->setDigitCount(6);

        gridLayout_2->addWidget(ForX1, 2, 3, 1, 1);

        label_64 = new QLabel(widget2);
        label_64->setObjectName(QStringLiteral("label_64"));

        gridLayout_2->addWidget(label_64, 5, 3, 1, 1);

        fakeWorldObj = new QTextEdit(centralWidget);
        fakeWorldObj->setObjectName(QStringLiteral("fakeWorldObj"));
        fakeWorldObj->setEnabled(true);
        fakeWorldObj->setGeometry(QRect(10, 10, 1051, 701));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(fakeWorldObj->sizePolicy().hasHeightForWidth());
        fakeWorldObj->setSizePolicy(sizePolicy);
        fakeWorldObj->setMinimumSize(QSize(1051, 701));
        fakeWorldObj->setMaximumSize(QSize(1051, 701));
        fakeWorldObj->setBaseSize(QSize(1051, 701));
        fakeWorldObj->setAutoFillBackground(false);
        widget3 = new QWidget(centralWidget);
        widget3->setObjectName(QStringLiteral("widget3"));
        widget3->setGeometry(QRect(1890, 10, 391, 251));
        gridLayout = new QGridLayout(widget3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setVerticalSpacing(20);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        StiffnessExp = new QPushButton(widget3);
        StiffnessExp->setObjectName(QStringLiteral("StiffnessExp"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(StiffnessExp->sizePolicy().hasHeightForWidth());
        StiffnessExp->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(StiffnessExp, 1, 3, 1, 1);

        HoxelExp = new QPushButton(widget3);
        HoxelExp->setObjectName(QStringLiteral("HoxelExp"));
        sizePolicy1.setHeightForWidth(HoxelExp->sizePolicy().hasHeightForWidth());
        HoxelExp->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(HoxelExp, 1, 2, 1, 1);

        Manual = new QPushButton(widget3);
        Manual->setObjectName(QStringLiteral("Manual"));
        sizePolicy1.setHeightForWidth(Manual->sizePolicy().hasHeightForWidth());
        Manual->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(Manual, 2, 3, 1, 1);

        StiffnMassCombined = new QPushButton(widget3);
        StiffnMassCombined->setObjectName(QStringLiteral("StiffnMassCombined"));
        sizePolicy1.setHeightForWidth(StiffnMassCombined->sizePolicy().hasHeightForWidth());
        StiffnMassCombined->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(StiffnMassCombined, 2, 2, 1, 1);

        FingerMappingExp = new QPushButton(widget3);
        FingerMappingExp->setObjectName(QStringLiteral("FingerMappingExp"));
        sizePolicy1.setHeightForWidth(FingerMappingExp->sizePolicy().hasHeightForWidth());
        FingerMappingExp->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(FingerMappingExp, 0, 3, 1, 1);

        dynamicEnvironment = new QPushButton(widget3);
        dynamicEnvironment->setObjectName(QStringLiteral("dynamicEnvironment"));
        sizePolicy1.setHeightForWidth(dynamicEnvironment->sizePolicy().hasHeightForWidth());
        dynamicEnvironment->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(dynamicEnvironment, 0, 2, 1, 1);

        widget4 = new QWidget(centralWidget);
        widget4->setObjectName(QStringLiteral("widget4"));
        widget4->setGeometry(QRect(1630, 30, 241, 111));
        gridLayout_3 = new QGridLayout(widget4);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_31 = new QLabel(widget4);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout_3->addWidget(label_31, 0, 0, 1, 1);

        label_22 = new QLabel(widget4);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_3->addWidget(label_22, 0, 1, 1, 1);

        lcdNumberGraphicsRate = new QLCDNumber(widget4);
        lcdNumberGraphicsRate->setObjectName(QStringLiteral("lcdNumberGraphicsRate"));
        lcdNumberGraphicsRate->setDigitCount(7);

        gridLayout_3->addWidget(lcdNumberGraphicsRate, 1, 0, 1, 1);

        lcdNumberHapticRate = new QLCDNumber(widget4);
        lcdNumberHapticRate->setObjectName(QStringLiteral("lcdNumberHapticRate"));
        lcdNumberHapticRate->setDigitCount(7);

        gridLayout_3->addWidget(lcdNumberHapticRate, 1, 1, 1, 1);

        widget5 = new QWidget(centralWidget);
        widget5->setObjectName(QStringLiteral("widget5"));
        widget5->setGeometry(QRect(1630, 160, 241, 321));
        gridLayout_8 = new QGridLayout(widget5);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        label_23 = new QLabel(widget5);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_8->addWidget(label_23, 1, 0, 1, 1);

        label_51 = new QLabel(widget5);
        label_51->setObjectName(QStringLiteral("label_51"));

        gridLayout_8->addWidget(label_51, 0, 0, 1, 1);

        trialNo = new QLCDNumber(widget5);
        trialNo->setObjectName(QStringLiteral("trialNo"));
        QPalette palette1;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        QBrush brush2(QColor(248, 248, 248, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        trialNo->setPalette(palette1);
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        trialNo->setFont(font1);
        trialNo->setAutoFillBackground(false);
        trialNo->setLineWidth(1);
        trialNo->setMidLineWidth(0);
        trialNo->setDigitCount(3);

        gridLayout_8->addWidget(trialNo, 1, 1, 1, 1);

        pairNo = new QLCDNumber(widget5);
        pairNo->setObjectName(QStringLiteral("pairNo"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Light, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pairNo->setPalette(palette2);
        QFont font2;
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setUnderline(false);
        pairNo->setFont(font2);
        pairNo->setSmallDecimalPoint(false);
        pairNo->setDigitCount(3);

        gridLayout_8->addWidget(pairNo, 4, 1, 1, 1);

        label_24 = new QLabel(widget5);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout_8->addWidget(label_24, 4, 0, 1, 1);

        subject_no = new QSpinBox(widget5);
        subject_no->setObjectName(QStringLiteral("subject_no"));
        QFont font3;
        font3.setPointSize(18);
        subject_no->setFont(font3);
        subject_no->setMaximum(99);
        subject_no->setSingleStep(1);
        subject_no->setValue(1);

        gridLayout_8->addWidget(subject_no, 0, 1, 1, 1);

        AdjustTrialNo = new QCheckBox(widget5);
        AdjustTrialNo->setObjectName(QStringLiteral("AdjustTrialNo"));

        gridLayout_8->addWidget(AdjustTrialNo, 8, 2, 1, 1);

        SetTrialNo = new QSpinBox(widget5);
        SetTrialNo->setObjectName(QStringLiteral("SetTrialNo"));
        SetTrialNo->setFont(font3);
        SetTrialNo->setMaximum(99);
        SetTrialNo->setSingleStep(1);
        SetTrialNo->setValue(1);

        gridLayout_8->addWidget(SetTrialNo, 8, 1, 1, 1);

        label_56 = new QLabel(widget5);
        label_56->setObjectName(QStringLiteral("label_56"));

        gridLayout_8->addWidget(label_56, 8, 0, 1, 1);

        widget6 = new QWidget(centralWidget);
        widget6->setObjectName(QStringLiteral("widget6"));
        widget6->setGeometry(QRect(1140, 255, 461, 221));
        gridLayout_7 = new QGridLayout(widget6);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget6);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_7->addWidget(label, 0, 0, 1, 1);

        label_26 = new QLabel(widget6);
        label_26->setObjectName(QStringLiteral("label_26"));

        gridLayout_7->addWidget(label_26, 2, 0, 1, 1);

        MotorLCD_1 = new QLCDNumber(widget6);
        MotorLCD_1->setObjectName(QStringLiteral("MotorLCD_1"));
        MotorLCD_1->setDigitCount(6);

        gridLayout_7->addWidget(MotorLCD_1, 1, 1, 1, 1);

        condition = new QLCDNumber(widget6);
        condition->setObjectName(QStringLiteral("condition"));
        condition->setDigitCount(6);

        gridLayout_7->addWidget(condition, 5, 1, 1, 1);

        StrokeLCD1_0 = new QLCDNumber(widget6);
        StrokeLCD1_0->setObjectName(QStringLiteral("StrokeLCD1_0"));
        StrokeLCD1_0->setDigitCount(6);

        gridLayout_7->addWidget(StrokeLCD1_0, 3, 0, 1, 1);

        label_37 = new QLabel(widget6);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_37, 4, 1, 1, 2);

        label_36 = new QLabel(widget6);
        label_36->setObjectName(QStringLiteral("label_36"));
        label_36->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_36, 4, 0, 1, 1);

        StrokeLCD1_1 = new QLCDNumber(widget6);
        StrokeLCD1_1->setObjectName(QStringLiteral("StrokeLCD1_1"));
        StrokeLCD1_1->setDigitCount(6);

        gridLayout_7->addWidget(StrokeLCD1_1, 3, 1, 1, 1);

        label_59 = new QLabel(widget6);
        label_59->setObjectName(QStringLiteral("label_59"));

        gridLayout_7->addWidget(label_59, 0, 1, 1, 2);

        label_27 = new QLabel(widget6);
        label_27->setObjectName(QStringLiteral("label_27"));

        gridLayout_7->addWidget(label_27, 2, 1, 1, 1);

        MotorLCD_0 = new QLCDNumber(widget6);
        MotorLCD_0->setObjectName(QStringLiteral("MotorLCD_0"));
        MotorLCD_0->setDigitCount(6);

        gridLayout_7->addWidget(MotorLCD_0, 1, 0, 1, 1);

        Condition1 = new QRadioButton(widget6);
        Condition1->setObjectName(QStringLiteral("Condition1"));

        gridLayout_7->addWidget(Condition1, 5, 2, 1, 1);

        direction = new QLCDNumber(widget6);
        direction->setObjectName(QStringLiteral("direction"));
        direction->setDigitCount(6);

        gridLayout_7->addWidget(direction, 5, 0, 1, 1);

        widget7 = new QWidget(centralWidget);
        widget7->setObjectName(QStringLiteral("widget7"));
        widget7->setGeometry(QRect(1890, 290, 191, 201));
        gridLayout_4 = new QGridLayout(widget7);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_14 = new QLabel(widget7);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setFont(font);
        label_14->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_14, 0, 0, 1, 2);

        label_16 = new QLabel(widget7);
        label_16->setObjectName(QStringLiteral("label_16"));
        QFont font4;
        font4.setPointSize(10);
        label_16->setFont(font4);
        label_16->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_16, 1, 0, 1, 1);

        label_17 = new QLabel(widget7);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setFont(font4);
        label_17->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_17, 1, 1, 1, 1);

        mass1_show = new QLCDNumber(widget7);
        mass1_show->setObjectName(QStringLiteral("mass1_show"));
        mass1_show->setDigitCount(6);

        gridLayout_4->addWidget(mass1_show, 2, 0, 1, 1);

        stiff1Scale_show = new QLCDNumber(widget7);
        stiff1Scale_show->setObjectName(QStringLiteral("stiff1Scale_show"));
        stiff1Scale_show->setDigitCount(6);

        gridLayout_4->addWidget(stiff1Scale_show, 2, 1, 1, 1);

        label_15 = new QLabel(widget7);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setFont(font);
        label_15->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_15, 3, 0, 1, 2);

        label_25 = new QLabel(widget7);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setFont(font4);
        label_25->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_25, 4, 0, 1, 1);

        label_18 = new QLabel(widget7);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setFont(font4);
        label_18->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_18, 4, 1, 1, 1);

        mass2_show = new QLCDNumber(widget7);
        mass2_show->setObjectName(QStringLiteral("mass2_show"));
        mass2_show->setDigitCount(6);

        gridLayout_4->addWidget(mass2_show, 5, 0, 1, 1);

        stiff2Scale_show = new QLCDNumber(widget7);
        stiff2Scale_show->setObjectName(QStringLiteral("stiff2Scale_show"));
        stiff2Scale_show->setDigitCount(6);

        gridLayout_4->addWidget(stiff2Scale_show, 5, 1, 1, 1);

        widget8 = new QWidget(centralWidget);
        widget8->setObjectName(QStringLiteral("widget8"));
        widget8->setGeometry(QRect(2120, 280, 121, 351));
        gridLayout_5 = new QGridLayout(widget8);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        label_35 = new QLabel(widget8);
        label_35->setObjectName(QStringLiteral("label_35"));

        gridLayout_5->addWidget(label_35, 0, 0, 1, 1);

        mass_set = new QLineEdit(widget8);
        mass_set->setObjectName(QStringLiteral("mass_set"));

        gridLayout_5->addWidget(mass_set, 1, 0, 1, 1);

        label_32 = new QLabel(widget8);
        label_32->setObjectName(QStringLiteral("label_32"));

        gridLayout_5->addWidget(label_32, 2, 0, 1, 1);

        cd_ratio = new QLineEdit(widget8);
        cd_ratio->setObjectName(QStringLiteral("cd_ratio"));

        gridLayout_5->addWidget(cd_ratio, 3, 0, 1, 1);

        label_11 = new QLabel(widget8);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_5->addWidget(label_11, 4, 0, 1, 1);

        inertiaValue = new QLineEdit(widget8);
        inertiaValue->setObjectName(QStringLiteral("inertiaValue"));

        gridLayout_5->addWidget(inertiaValue, 5, 0, 1, 1);

        VRControl = new QRadioButton(widget8);
        VRControl->setObjectName(QStringLiteral("VRControl"));

        gridLayout_5->addWidget(VRControl, 6, 0, 1, 1);

        equalBox = new QRadioButton(widget8);
        equalBox->setObjectName(QStringLiteral("equalBox"));

        gridLayout_5->addWidget(equalBox, 7, 0, 1, 1);

        equalBoxTouch = new QCheckBox(widget8);
        equalBoxTouch->setObjectName(QStringLiteral("equalBoxTouch"));

        gridLayout_5->addWidget(equalBoxTouch, 8, 0, 1, 1);

        widget9 = new QWidget(centralWidget);
        widget9->setObjectName(QStringLiteral("widget9"));
        widget9->setGeometry(QRect(1130, 500, 518, 194));
        horizontalLayout = new QHBoxLayout(widget9);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        text = new QTextBrowser(widget9);
        text->setObjectName(QStringLiteral("text"));

        horizontalLayout->addWidget(text);

        mappingTextBox = new QTextBrowser(widget9);
        mappingTextBox->setObjectName(QStringLiteral("mappingTextBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mappingTextBox->sizePolicy().hasHeightForWidth());
        mappingTextBox->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(mappingTextBox);

        widget10 = new QWidget(centralWidget);
        widget10->setObjectName(QStringLiteral("widget10"));
        widget10->setGeometry(QRect(1980, 510, 111, 201));
        gridLayout_10 = new QGridLayout(widget10);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        turnOff = new QPushButton(widget10);
        turnOff->setObjectName(QStringLiteral("turnOff"));

        gridLayout_10->addWidget(turnOff, 0, 0, 1, 1);

        JakeRenderCheckBox = new QCheckBox(widget10);
        JakeRenderCheckBox->setObjectName(QStringLiteral("JakeRenderCheckBox"));

        gridLayout_10->addWidget(JakeRenderCheckBox, 1, 0, 1, 1);

        reverseBox = new QRadioButton(widget10);
        reverseBox->setObjectName(QStringLiteral("reverseBox"));

        gridLayout_10->addWidget(reverseBox, 2, 0, 1, 1);

        CD_Vertical_Box = new QCheckBox(widget10);
        CD_Vertical_Box->setObjectName(QStringLiteral("CD_Vertical_Box"));

        gridLayout_10->addWidget(CD_Vertical_Box, 3, 0, 1, 1);

        widget11 = new QWidget(centralWidget);
        widget11->setObjectName(QStringLiteral("widget11"));
        widget11->setGeometry(QRect(1700, 630, 261, 81));
        gridLayout_9 = new QGridLayout(widget11);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        label_54 = new QLabel(widget11);
        label_54->setObjectName(QStringLiteral("label_54"));
        label_54->setAlignment(Qt::AlignCenter);

        gridLayout_9->addWidget(label_54, 0, 0, 1, 1);

        Act1_show = new QLCDNumber(widget11);
        Act1_show->setObjectName(QStringLiteral("Act1_show"));
        Act1_show->setDigitCount(6);

        gridLayout_9->addWidget(Act1_show, 0, 1, 1, 1);

        Act1 = new QSlider(widget11);
        Act1->setObjectName(QStringLiteral("Act1"));
        Act1->setMinimum(0);
        Act1->setMaximum(90);
        Act1->setSingleStep(1);
        Act1->setPageStep(10);
        Act1->setValue(0);
        Act1->setSliderPosition(0);
        Act1->setOrientation(Qt::Horizontal);
        Act1->setTickPosition(QSlider::NoTicks);

        gridLayout_9->addWidget(Act1, 0, 2, 1, 1);

        label_55 = new QLabel(widget11);
        label_55->setObjectName(QStringLiteral("label_55"));
        label_55->setAlignment(Qt::AlignCenter);

        gridLayout_9->addWidget(label_55, 1, 0, 1, 1);

        Act2_show = new QLCDNumber(widget11);
        Act2_show->setObjectName(QStringLiteral("Act2_show"));
        Act2_show->setDigitCount(6);

        gridLayout_9->addWidget(Act2_show, 1, 1, 1, 1);

        Act2 = new QSlider(widget11);
        Act2->setObjectName(QStringLiteral("Act2"));
        Act2->setMinimum(0);
        Act2->setMaximum(9);
        Act2->setSingleStep(1);
        Act2->setPageStep(10);
        Act2->setValue(0);
        Act2->setSliderPosition(0);
        Act2->setOrientation(Qt::Horizontal);
        Act2->setTickPosition(QSlider::NoTicks);

        gridLayout_9->addWidget(Act2, 1, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        fakeWorldObj->raise();
        widget->raise();
        label_20->raise();
        label_67->raise();
        label_64->raise();
        label_19->raise();
        trialNo->raise();
        label_23->raise();
        DesZ0->raise();
        VRControl->raise();
        label_21->raise();
        label_24->raise();
        label_59->raise();
        label->raise();
        dynamicEnvironment->raise();
        lcdNumberHapticRate->raise();
        turnOff->raise();
        DesY0->raise();
        label_69->raise();
        label_22->raise();
        DesX1->raise();
        DesZ1->raise();
        DesY1->raise();
        DesX0->raise();
        lcdNumberGraphicsRate->raise();
        label_31->raise();
        JakeRenderCheckBox->raise();
        label_11->raise();
        inertiaValue->raise();
        label_32->raise();
        cd_ratio->raise();
        CD_Vertical_Box->raise();
        StiffnessExp->raise();
        label_35->raise();
        mass_set->raise();
        MotorLCD_1->raise();
        StrokeLCD1_1->raise();
        label_27->raise();
        StrokeLCD1_0->raise();
        label_26->raise();
        MotorLCD_0->raise();
        pairNo->raise();
        label_49->raise();
        label_50->raise();
        equalBoxTouch->raise();
        label_14->raise();
        stiff1Scale_show->raise();
        reverseBox->raise();
        label_51->raise();
        subject_no->raise();
        stiff2Scale_show->raise();
        text->raise();
        Act1_show->raise();
        Act2_show->raise();
        Act2->raise();
        label_54->raise();
        Act1->raise();
        label_55->raise();
        ForZ0->raise();
        ForY0->raise();
        ForX0->raise();
        Manual->raise();
        direction->raise();
        ForY1->raise();
        ForX1->raise();
        ForZ1->raise();
        label_52->raise();
        label_53->raise();
        ForY0_2->raise();
        ForX0_2->raise();
        ForX1_2->raise();
        ForZ0_2->raise();
        ForZ1_2->raise();
        ForY1_2->raise();
        mass1_show->raise();
        mass2_show->raise();
        equalBox->raise();
        StiffnMassCombined->raise();
        SetTrialNo->raise();
        label_56->raise();
        AdjustTrialNo->raise();
        FingerMappingExp->raise();
        label_57->raise();
        label_58->raise();
        Condition1->raise();
        condition->raise();
        label_36->raise();
        label_37->raise();
        label_16->raise();
        label_17->raise();
        label_18->raise();
        label_15->raise();
        label_25->raise();
        mappingTextBox->raise();
        HoxelExp->raise();
        placeHolder->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 2300, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CHAI3D Haptics Environment", 0));
        label_12->setText(QApplication::translate("MainWindow", "Mass", 0));
        label_33->setText(QApplication::translate("MainWindow", "CD", 0));
        label_34->setText(QApplication::translate("MainWindow", "Inertia", 0));
        label_21->setText(QApplication::translate("MainWindow", "Des Z", 0));
        label_52->setText(QApplication::translate("MainWindow", "Stiffness Forces:", 0));
        label_50->setText(QApplication::translate("MainWindow", "THUMB", 0));
        label_57->setText(QApplication::translate("MainWindow", "Stiffness Forces:", 0));
        label_20->setText(QApplication::translate("MainWindow", "Des Y", 0));
        label_67->setText(QApplication::translate("MainWindow", "Des Y", 0));
        label_69->setText(QApplication::translate("MainWindow", "Des Z", 0));
        label_19->setText(QApplication::translate("MainWindow", "Des X", 0));
        label_53->setText(QApplication::translate("MainWindow", "Mass Forces:", 0));
        label_49->setText(QApplication::translate("MainWindow", "INDEX", 0));
        label_58->setText(QApplication::translate("MainWindow", "Mass Forces:", 0));
        label_64->setText(QApplication::translate("MainWindow", "Des X", 0));
        StiffnessExp->setText(QApplication::translate("MainWindow", "Stiffness\n"
"Experiment", 0));
        HoxelExp->setText(QApplication::translate("MainWindow", "Hoxel\n"
"Experiment", 0));
        Manual->setText(QApplication::translate("MainWindow", "Manual", 0));
        StiffnMassCombined->setText(QApplication::translate("MainWindow", "Stiffness-Mass\n"
"Experiment", 0));
        FingerMappingExp->setText(QApplication::translate("MainWindow", "Finger Mapping\n"
"Experiment", 0));
        dynamicEnvironment->setText(QApplication::translate("MainWindow", "Dynamic\n"
"Bodies", 0));
        label_31->setText(QApplication::translate("MainWindow", "Graphics\n"
"Rate:", 0));
        label_22->setText(QApplication::translate("MainWindow", "Haptic\n"
"Rate:", 0));
        label_23->setText(QApplication::translate("MainWindow", "Trial No.", 0));
        label_51->setText(QApplication::translate("MainWindow", "Subject No.", 0));
        label_24->setText(QApplication::translate("MainWindow", "Pair No.", 0));
        AdjustTrialNo->setText(QApplication::translate("MainWindow", "Adjust\n"
"Trial No.", 0));
        label_56->setText(QApplication::translate("MainWindow", "Trial No.", 0));
        label->setText(QApplication::translate("MainWindow", "Index Pot", 0));
        label_26->setText(QApplication::translate("MainWindow", "Desired", 0));
        label_37->setText(QApplication::translate("MainWindow", "Condtion No.:", 0));
        label_36->setText(QApplication::translate("MainWindow", "Direction:", 0));
        label_59->setText(QApplication::translate("MainWindow", "Sensor", 0));
        label_27->setText(QApplication::translate("MainWindow", "Desired", 0));
        Condition1->setText(QApplication::translate("MainWindow", "Cond1", 0));
        label_14->setText(QApplication::translate("MainWindow", "Box1", 0));
        label_16->setText(QApplication::translate("MainWindow", "Mass", 0));
        label_17->setText(QApplication::translate("MainWindow", "Stiffness", 0));
        label_15->setText(QApplication::translate("MainWindow", "Box2", 0));
        label_25->setText(QApplication::translate("MainWindow", "Mass", 0));
        label_18->setText(QApplication::translate("MainWindow", "Stiffness", 0));
        label_35->setText(QApplication::translate("MainWindow", "Mass", 0));
        label_32->setText(QApplication::translate("MainWindow", "CD Ratio", 0));
        label_11->setText(QApplication::translate("MainWindow", "Scaled\n"
"Gravity", 0));
        VRControl->setText(QApplication::translate("MainWindow", "VR\n"
"Control", 0));
        equalBox->setText(QApplication::translate("MainWindow", "Equal", 0));
        equalBoxTouch->setText(QApplication::translate("MainWindow", "Touch\n"
"Effective", 0));
        turnOff->setText(QApplication::translate("MainWindow", "Turn Off", 0));
        JakeRenderCheckBox->setText(QApplication::translate("MainWindow", "JakeRender", 0));
        reverseBox->setText(QApplication::translate("MainWindow", "Reverse", 0));
        CD_Vertical_Box->setText(QApplication::translate("MainWindow", "CD Vertical", 0));
        label_54->setText(QApplication::translate("MainWindow", "Act1", 0));
        label_55->setText(QApplication::translate("MainWindow", "Act2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
