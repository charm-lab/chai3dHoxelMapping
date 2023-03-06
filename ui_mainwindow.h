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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
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
    QTextBrowser *text;
    QTextBrowser *mappingTextBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_5;
    QLabel *label_16;
    QLabel *label_14;
    QLCDNumber *mass1_show;
    QLCDNumber *stiff1Scale_show;
    QLabel *label_15;
    QLCDNumber *mass2_show;
    QLCDNumber *stiff2Scale_show;
    QLabel *label_30;
    QLCDNumber *mass3_show;
    QLCDNumber *stiff3Scale_show;
    QLabel *label_17;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_4;
    QLabel *label_31;
    QLabel *label_22;
    QLCDNumber *lcdNumberGraphicsRate;
    QLCDNumber *lcdNumberHapticRate;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_9;
    QCheckBox *AdjustTrialNo;
    QSpinBox *subject_no;
    QLabel *label_23;
    QLCDNumber *trialNo;
    QLCDNumber *pairNo;
    QSpinBox *SetTrialNo;
    QLabel *label_24;
    QLabel *label_56;
    QLabel *label_51;
    QCheckBox *planarCheckBox;
    QGroupBox *groupBox;
    QRadioButton *forceControlButton;
    QRadioButton *positionControlButton;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout_11;
    QDoubleSpinBox *cameraRadius_Box;
    QLabel *label_65;
    QDoubleSpinBox *cameraPolar_Box;
    QDoubleSpinBox *cameraAzimuth_Box;
    QPushButton *resetCameraButton;
    QLabel *label_62;
    QLabel *label_63;
    QCheckBox *lockCameraBox;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget4;
    QGridLayout *gridLayout_2;
    QComboBox *PortBox;
    QComboBox *ParityBox;
    QLabel *label_41;
    QComboBox *BitNumBox;
    QLabel *label_40;
    QComboBox *StopBox;
    QLabel *label_43;
    QLabel *label_42;
    QComboBox *BaudBox;
    QLabel *label_39;
    QPushButton *openButton;
    QWidget *layoutWidget5;
    QGridLayout *gridLayout_6;
    QLabel *label_44;
    QTextEdit *serialWrite1;
    QLabel *label_45;
    QTextEdit *serialWrite2;
    QTextEdit *serialRead;
    QLabel *label_49;
    QLabel *label_50;
    QGroupBox *groupBox_4;
    QWidget *layoutWidget6;
    QGridLayout *gridLayout_7;
    QLabel *label_55;
    QLineEdit *mass_set;
    QLCDNumber *Act1_show;
    QLabel *label_60;
    QLabel *label_11;
    QLabel *label_32;
    QRadioButton *equalBox;
    QCheckBox *JakeRenderCheckBox;
    QSlider *Act1;
    QCheckBox *CD_Vertical_Box;
    QLabel *label_35;
    QLCDNumber *Act2_show;
    QLabel *label_54;
    QLCDNumber *Inertia_Val;
    QSpacerItem *verticalSpacer;
    QLineEdit *cd_ratio;
    QPushButton *turnOff;
    QRadioButton *VRControl;
    QSlider *Act2;
    QLCDNumber *Mass_Val;
    QRadioButton *reverseBox;
    QLabel *label_48;
    QLCDNumber *CD_Val;
    QLabel *label_28;
    QLineEdit *inertiaValue;
    QSpacerItem *verticalSpacer_2;
    QCheckBox *equalBoxTouch;
    QPushButton *Manual;
    QCheckBox *showAllFramesCheckBox;
    QGroupBox *groupBox_5;
    QWidget *layoutWidget7;
    QGridLayout *gridLayout_12;
    QPushButton *FingerMappingExp;
    QPushButton *dynamicEnvironment;
    QPushButton *HoxelMappingExp;
    QPushButton *MultiMassExp;
    QPushButton *CubeGuidanceExp;
    QGroupBox *groupBox_6;
    QWidget *layoutWidget8;
    QGridLayout *gridLayout;
    QPushButton *StiffnessExp;
    QPushButton *StiffnMassCombined;
    QGroupBox *groupBox_7;
    QWidget *layoutWidget9;
    QGridLayout *gridLayout_10;
    QLabel *label_52;
    QLCDNumber *ForX0;
    QLCDNumber *ForY0;
    QLCDNumber *ForZ0;
    QLabel *label_53;
    QLCDNumber *ForX0_2;
    QLCDNumber *ForY0_2;
    QLCDNumber *ForZ0_2;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLCDNumber *DesX0;
    QLCDNumber *DesY0;
    QLCDNumber *DesZ0;
    QWidget *layoutWidget10;
    QGridLayout *gridLayout_16;
    QLabel *label;
    QLabel *label_26;
    QLCDNumber *MotorLCD_0;
    QLCDNumber *StrokeLCD1_0;
    QGroupBox *groupBox_8;
    QWidget *layoutWidget11;
    QGridLayout *gridLayout_14;
    QLabel *label_57;
    QLCDNumber *ForX1;
    QLCDNumber *ForY1;
    QLCDNumber *ForZ1;
    QLabel *label_58;
    QLCDNumber *ForX1_2;
    QLCDNumber *ForY1_2;
    QLCDNumber *ForZ1_2;
    QLabel *label_64;
    QLabel *label_67;
    QLabel *label_69;
    QLCDNumber *DesX1;
    QLCDNumber *DesY1;
    QLCDNumber *DesZ1;
    QWidget *layoutWidget12;
    QGridLayout *gridLayout_15;
    QLabel *label_59;
    QLabel *label_27;
    QLCDNumber *MotorLCD_1;
    QLCDNumber *StrokeLCD1_1;
    QWidget *layoutWidget13;
    QGridLayout *gridLayout_3;
    QLabel *label_36;
    QLabel *label_37;
    QLCDNumber *direction;
    QLCDNumber *condition;
    QRadioButton *Condition1;
    QGroupBox *groupBox_9;
    QWidget *layoutWidget14;
    QGridLayout *gridLayout_8;
    QLabel *label_46;
    QDoubleSpinBox *alphaBox;
    QPushButton *SetTrialNoButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(2540, 1356);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
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
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        placeHolder = new QWidget(centralWidget);
        placeHolder->setObjectName(QStringLiteral("placeHolder"));
        placeHolder->setGeometry(QRect(0, 0, 100, 30));
        text = new QTextBrowser(centralWidget);
        text->setObjectName(QStringLiteral("text"));
        text->setGeometry(QRect(890, 670, 260, 192));
        mappingTextBox = new QTextBrowser(centralWidget);
        mappingTextBox->setObjectName(QStringLiteral("mappingTextBox"));
        mappingTextBox->setGeometry(QRect(890, 890, 260, 91));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(1920, 160, 301, 241));
        layoutWidget->setFont(font);
        gridLayout_5 = new QGridLayout(layoutWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        label_16 = new QLabel(layoutWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setFont(font);
        label_16->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_16, 0, 1, 1, 1);

        label_14 = new QLabel(layoutWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setFont(font);
        label_14->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_14, 1, 0, 1, 1);

        mass1_show = new QLCDNumber(layoutWidget);
        mass1_show->setObjectName(QStringLiteral("mass1_show"));
        mass1_show->setFont(font);
        mass1_show->setDigitCount(6);

        gridLayout_5->addWidget(mass1_show, 1, 1, 1, 1);

        stiff1Scale_show = new QLCDNumber(layoutWidget);
        stiff1Scale_show->setObjectName(QStringLiteral("stiff1Scale_show"));
        stiff1Scale_show->setFont(font);
        stiff1Scale_show->setDigitCount(6);

        gridLayout_5->addWidget(stiff1Scale_show, 1, 2, 1, 1);

        label_15 = new QLabel(layoutWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setFont(font);
        label_15->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_15, 2, 0, 1, 1);

        mass2_show = new QLCDNumber(layoutWidget);
        mass2_show->setObjectName(QStringLiteral("mass2_show"));
        mass2_show->setFont(font);
        mass2_show->setDigitCount(6);

        gridLayout_5->addWidget(mass2_show, 2, 1, 1, 1);

        stiff2Scale_show = new QLCDNumber(layoutWidget);
        stiff2Scale_show->setObjectName(QStringLiteral("stiff2Scale_show"));
        stiff2Scale_show->setFont(font);
        stiff2Scale_show->setDigitCount(6);

        gridLayout_5->addWidget(stiff2Scale_show, 2, 2, 1, 1);

        label_30 = new QLabel(layoutWidget);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setFont(font);
        label_30->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_30, 3, 0, 1, 1);

        mass3_show = new QLCDNumber(layoutWidget);
        mass3_show->setObjectName(QStringLiteral("mass3_show"));
        mass3_show->setFont(font);
        mass3_show->setDigitCount(6);

        gridLayout_5->addWidget(mass3_show, 3, 1, 1, 1);

        stiff3Scale_show = new QLCDNumber(layoutWidget);
        stiff3Scale_show->setObjectName(QStringLiteral("stiff3Scale_show"));
        stiff3Scale_show->setFont(font);
        stiff3Scale_show->setDigitCount(6);

        gridLayout_5->addWidget(stiff3Scale_show, 3, 2, 1, 1);

        label_17 = new QLabel(layoutWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setFont(font);
        label_17->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_17, 0, 2, 1, 1);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(1920, 20, 301, 131));
        gridLayout_4 = new QGridLayout(layoutWidget1);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_31 = new QLabel(layoutWidget1);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout_4->addWidget(label_31, 0, 0, 1, 1);

        label_22 = new QLabel(layoutWidget1);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_4->addWidget(label_22, 0, 1, 1, 1);

        lcdNumberGraphicsRate = new QLCDNumber(layoutWidget1);
        lcdNumberGraphicsRate->setObjectName(QStringLiteral("lcdNumberGraphicsRate"));
        lcdNumberGraphicsRate->setDigitCount(7);

        gridLayout_4->addWidget(lcdNumberGraphicsRate, 1, 0, 1, 1);

        lcdNumberHapticRate = new QLCDNumber(layoutWidget1);
        lcdNumberHapticRate->setObjectName(QStringLiteral("lcdNumberHapticRate"));
        lcdNumberHapticRate->setDigitCount(7);

        gridLayout_4->addWidget(lcdNumberHapticRate, 1, 1, 1, 1);

        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(1230, 670, 381, 301));
        gridLayout_9 = new QGridLayout(layoutWidget2);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        AdjustTrialNo = new QCheckBox(layoutWidget2);
        AdjustTrialNo->setObjectName(QStringLiteral("AdjustTrialNo"));

        gridLayout_9->addWidget(AdjustTrialNo, 6, 1, 1, 1);

        subject_no = new QSpinBox(layoutWidget2);
        subject_no->setObjectName(QStringLiteral("subject_no"));
        QFont font1;
        font1.setPointSize(18);
        subject_no->setFont(font1);
        subject_no->setMaximum(99);
        subject_no->setSingleStep(1);
        subject_no->setValue(1);

        gridLayout_9->addWidget(subject_no, 2, 0, 1, 1);

        label_23 = new QLabel(layoutWidget2);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_9->addWidget(label_23, 3, 0, 1, 1);

        trialNo = new QLCDNumber(layoutWidget2);
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
        QFont font2;
        font2.setPointSize(14);
        font2.setBold(true);
        trialNo->setFont(font2);
        trialNo->setAutoFillBackground(false);
        trialNo->setLineWidth(1);
        trialNo->setMidLineWidth(0);
        trialNo->setDigitCount(3);

        gridLayout_9->addWidget(trialNo, 4, 0, 1, 1);

        pairNo = new QLCDNumber(layoutWidget2);
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
        QFont font3;
        font3.setPointSize(16);
        font3.setBold(true);
        font3.setUnderline(false);
        pairNo->setFont(font3);
        pairNo->setSmallDecimalPoint(false);
        pairNo->setDigitCount(3);

        gridLayout_9->addWidget(pairNo, 4, 1, 1, 1);

        SetTrialNo = new QSpinBox(layoutWidget2);
        SetTrialNo->setObjectName(QStringLiteral("SetTrialNo"));
        SetTrialNo->setFont(font1);
        SetTrialNo->setMaximum(99);
        SetTrialNo->setSingleStep(1);
        SetTrialNo->setValue(1);

        gridLayout_9->addWidget(SetTrialNo, 6, 0, 1, 1);

        label_24 = new QLabel(layoutWidget2);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout_9->addWidget(label_24, 3, 1, 1, 1);

        label_56 = new QLabel(layoutWidget2);
        label_56->setObjectName(QStringLiteral("label_56"));

        gridLayout_9->addWidget(label_56, 5, 0, 1, 1);

        label_51 = new QLabel(layoutWidget2);
        label_51->setObjectName(QStringLiteral("label_51"));

        gridLayout_9->addWidget(label_51, 1, 0, 1, 1);

        planarCheckBox = new QCheckBox(centralWidget);
        planarCheckBox->setObjectName(QStringLiteral("planarCheckBox"));
        planarCheckBox->setGeometry(QRect(1780, 1020, 321, 121));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(planarCheckBox->sizePolicy().hasHeightForWidth());
        planarCheckBox->setSizePolicy(sizePolicy1);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(1410, 1020, 341, 271));
        forceControlButton = new QRadioButton(groupBox);
        forceControlButton->setObjectName(QStringLiteral("forceControlButton"));
        forceControlButton->setGeometry(QRect(20, 40, 191, 101));
        positionControlButton = new QRadioButton(groupBox);
        positionControlButton->setObjectName(QStringLiteral("positionControlButton"));
        positionControlButton->setGeometry(QRect(20, 150, 191, 101));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(880, 1020, 511, 271));
        layoutWidget3 = new QWidget(groupBox_2);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 40, 491, 221));
        gridLayout_11 = new QGridLayout(layoutWidget3);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        gridLayout_11->setContentsMargins(0, 0, 0, 0);
        cameraRadius_Box = new QDoubleSpinBox(layoutWidget3);
        cameraRadius_Box->setObjectName(QStringLiteral("cameraRadius_Box"));
        sizePolicy1.setHeightForWidth(cameraRadius_Box->sizePolicy().hasHeightForWidth());
        cameraRadius_Box->setSizePolicy(sizePolicy1);
        cameraRadius_Box->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        cameraRadius_Box->setMaximum(360);
        cameraRadius_Box->setSingleStep(0.05);
        cameraRadius_Box->setValue(1);

        gridLayout_11->addWidget(cameraRadius_Box, 3, 1, 1, 1);

        label_65 = new QLabel(layoutWidget3);
        label_65->setObjectName(QStringLiteral("label_65"));
        sizePolicy1.setHeightForWidth(label_65->sizePolicy().hasHeightForWidth());
        label_65->setSizePolicy(sizePolicy1);
        label_65->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_11->addWidget(label_65, 2, 0, 1, 1);

        cameraPolar_Box = new QDoubleSpinBox(layoutWidget3);
        cameraPolar_Box->setObjectName(QStringLiteral("cameraPolar_Box"));
        sizePolicy1.setHeightForWidth(cameraPolar_Box->sizePolicy().hasHeightForWidth());
        cameraPolar_Box->setSizePolicy(sizePolicy1);
        cameraPolar_Box->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        cameraPolar_Box->setMinimum(-360);
        cameraPolar_Box->setMaximum(360);
        cameraPolar_Box->setSingleStep(1);
        cameraPolar_Box->setValue(1);

        gridLayout_11->addWidget(cameraPolar_Box, 1, 1, 1, 1);

        cameraAzimuth_Box = new QDoubleSpinBox(layoutWidget3);
        cameraAzimuth_Box->setObjectName(QStringLiteral("cameraAzimuth_Box"));
        sizePolicy1.setHeightForWidth(cameraAzimuth_Box->sizePolicy().hasHeightForWidth());
        cameraAzimuth_Box->setSizePolicy(sizePolicy1);
        cameraAzimuth_Box->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        cameraAzimuth_Box->setMinimum(-360);
        cameraAzimuth_Box->setMaximum(360);
        cameraAzimuth_Box->setSingleStep(1);
        cameraAzimuth_Box->setValue(1);

        gridLayout_11->addWidget(cameraAzimuth_Box, 2, 1, 1, 1);

        resetCameraButton = new QPushButton(layoutWidget3);
        resetCameraButton->setObjectName(QStringLiteral("resetCameraButton"));

        gridLayout_11->addWidget(resetCameraButton, 4, 0, 1, 1);

        label_62 = new QLabel(layoutWidget3);
        label_62->setObjectName(QStringLiteral("label_62"));
        sizePolicy1.setHeightForWidth(label_62->sizePolicy().hasHeightForWidth());
        label_62->setSizePolicy(sizePolicy1);
        label_62->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_11->addWidget(label_62, 3, 0, 1, 1);

        label_63 = new QLabel(layoutWidget3);
        label_63->setObjectName(QStringLiteral("label_63"));
        sizePolicy1.setHeightForWidth(label_63->sizePolicy().hasHeightForWidth());
        label_63->setSizePolicy(sizePolicy1);
        label_63->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_11->addWidget(label_63, 1, 0, 1, 1);

        lockCameraBox = new QCheckBox(layoutWidget3);
        lockCameraBox->setObjectName(QStringLiteral("lockCameraBox"));
        lockCameraBox->setEnabled(true);
        lockCameraBox->setChecked(true);

        gridLayout_11->addWidget(lockCameraBox, 4, 1, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(110, 510, 691, 711));
        layoutWidget4 = new QWidget(groupBox_3);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(160, 60, 281, 205));
        gridLayout_2 = new QGridLayout(layoutWidget4);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        PortBox = new QComboBox(layoutWidget4);
        PortBox->setObjectName(QStringLiteral("PortBox"));

        gridLayout_2->addWidget(PortBox, 0, 1, 1, 1);

        ParityBox = new QComboBox(layoutWidget4);
        ParityBox->setObjectName(QStringLiteral("ParityBox"));
        ParityBox->setEditable(true);

        gridLayout_2->addWidget(ParityBox, 3, 1, 1, 1);

        label_41 = new QLabel(layoutWidget4);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_41, 2, 0, 1, 1);

        BitNumBox = new QComboBox(layoutWidget4);
        BitNumBox->setObjectName(QStringLiteral("BitNumBox"));
        BitNumBox->setEditable(true);

        gridLayout_2->addWidget(BitNumBox, 2, 1, 1, 1);

        label_40 = new QLabel(layoutWidget4);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_40, 1, 0, 1, 1);

        StopBox = new QComboBox(layoutWidget4);
        StopBox->setObjectName(QStringLiteral("StopBox"));
        StopBox->setEditable(true);

        gridLayout_2->addWidget(StopBox, 4, 1, 1, 1);

        label_43 = new QLabel(layoutWidget4);
        label_43->setObjectName(QStringLiteral("label_43"));
        label_43->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_43, 4, 0, 1, 1);

        label_42 = new QLabel(layoutWidget4);
        label_42->setObjectName(QStringLiteral("label_42"));
        label_42->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_42, 3, 0, 1, 1);

        BaudBox = new QComboBox(layoutWidget4);
        BaudBox->setObjectName(QStringLiteral("BaudBox"));
        BaudBox->setEditable(true);

        gridLayout_2->addWidget(BaudBox, 1, 1, 1, 1);

        label_39 = new QLabel(layoutWidget4);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_39, 0, 0, 1, 1);

        openButton = new QPushButton(groupBox_3);
        openButton->setObjectName(QStringLiteral("openButton"));
        openButton->setGeometry(QRect(10, 50, 141, 211));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(openButton->sizePolicy().hasHeightForWidth());
        openButton->setSizePolicy(sizePolicy2);
        layoutWidget5 = new QWidget(groupBox_3);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(10, 280, 641, 421));
        gridLayout_6 = new QGridLayout(layoutWidget5);
        gridLayout_6->setSpacing(10);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(10, 10, 10, 10);
        label_44 = new QLabel(layoutWidget5);
        label_44->setObjectName(QStringLiteral("label_44"));
        label_44->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_44, 1, 0, 1, 1);

        serialWrite1 = new QTextEdit(layoutWidget5);
        serialWrite1->setObjectName(QStringLiteral("serialWrite1"));

        gridLayout_6->addWidget(serialWrite1, 1, 1, 1, 1);

        label_45 = new QLabel(layoutWidget5);
        label_45->setObjectName(QStringLiteral("label_45"));
        label_45->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(label_45, 2, 0, 1, 1);

        serialWrite2 = new QTextEdit(layoutWidget5);
        serialWrite2->setObjectName(QStringLiteral("serialWrite2"));

        gridLayout_6->addWidget(serialWrite2, 2, 1, 1, 1);

        serialRead = new QTextEdit(layoutWidget5);
        serialRead->setObjectName(QStringLiteral("serialRead"));

        gridLayout_6->addWidget(serialRead, 4, 1, 1, 1);

        label_49 = new QLabel(layoutWidget5);
        label_49->setObjectName(QStringLiteral("label_49"));
        label_49->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_49, 3, 0, 1, 1);

        label_50 = new QLabel(layoutWidget5);
        label_50->setObjectName(QStringLiteral("label_50"));
        label_50->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_50, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(2160, 940, 351, 351));
        layoutWidget6 = new QWidget(groupBox_4);
        layoutWidget6->setObjectName(QStringLiteral("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(10, 30, 331, 309));
        QFont font4;
        font4.setPointSize(8);
        layoutWidget6->setFont(font4);
        gridLayout_7 = new QGridLayout(layoutWidget6);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        label_55 = new QLabel(layoutWidget6);
        label_55->setObjectName(QStringLiteral("label_55"));
        label_55->setFont(font4);
        label_55->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(label_55, 13, 0, 1, 1);

        mass_set = new QLineEdit(layoutWidget6);
        mass_set->setObjectName(QStringLiteral("mass_set"));
        mass_set->setFont(font4);

        gridLayout_7->addWidget(mass_set, 4, 3, 2, 1);

        Act1_show = new QLCDNumber(layoutWidget6);
        Act1_show->setObjectName(QStringLiteral("Act1_show"));
        Act1_show->setFont(font4);
        Act1_show->setDigitCount(6);

        gridLayout_7->addWidget(Act1_show, 12, 1, 1, 1);

        label_60 = new QLabel(layoutWidget6);
        label_60->setObjectName(QStringLiteral("label_60"));
        label_60->setFont(font4);
        label_60->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(label_60, 0, 2, 1, 1);

        label_11 = new QLabel(layoutWidget6);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setFont(font4);

        gridLayout_7->addWidget(label_11, 8, 3, 2, 1);

        label_32 = new QLabel(layoutWidget6);
        label_32->setObjectName(QStringLiteral("label_32"));
        label_32->setFont(font4);

        gridLayout_7->addWidget(label_32, 6, 3, 1, 1);

        equalBox = new QRadioButton(layoutWidget6);
        equalBox->setObjectName(QStringLiteral("equalBox"));
        sizePolicy2.setHeightForWidth(equalBox->sizePolicy().hasHeightForWidth());
        equalBox->setSizePolicy(sizePolicy2);
        equalBox->setFont(font4);

        gridLayout_7->addWidget(equalBox, 5, 2, 2, 1);

        JakeRenderCheckBox = new QCheckBox(layoutWidget6);
        JakeRenderCheckBox->setObjectName(QStringLiteral("JakeRenderCheckBox"));
        JakeRenderCheckBox->setFont(font4);

        gridLayout_7->addWidget(JakeRenderCheckBox, 5, 0, 2, 2);

        Act1 = new QSlider(layoutWidget6);
        Act1->setObjectName(QStringLiteral("Act1"));
        Act1->setFont(font4);
        Act1->setMinimum(0);
        Act1->setMaximum(90);
        Act1->setSingleStep(1);
        Act1->setPageStep(10);
        Act1->setValue(0);
        Act1->setSliderPosition(0);
        Act1->setOrientation(Qt::Horizontal);
        Act1->setTickPosition(QSlider::NoTicks);

        gridLayout_7->addWidget(Act1, 12, 2, 1, 1);

        CD_Vertical_Box = new QCheckBox(layoutWidget6);
        CD_Vertical_Box->setObjectName(QStringLiteral("CD_Vertical_Box"));
        CD_Vertical_Box->setFont(font4);

        gridLayout_7->addWidget(CD_Vertical_Box, 8, 0, 2, 2);

        label_35 = new QLabel(layoutWidget6);
        label_35->setObjectName(QStringLiteral("label_35"));
        label_35->setFont(font4);

        gridLayout_7->addWidget(label_35, 3, 3, 1, 1);

        Act2_show = new QLCDNumber(layoutWidget6);
        Act2_show->setObjectName(QStringLiteral("Act2_show"));
        Act2_show->setFont(font4);
        Act2_show->setDigitCount(6);

        gridLayout_7->addWidget(Act2_show, 13, 1, 1, 1);

        label_54 = new QLabel(layoutWidget6);
        label_54->setObjectName(QStringLiteral("label_54"));
        label_54->setFont(font4);
        label_54->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(label_54, 12, 0, 1, 1);

        Inertia_Val = new QLCDNumber(layoutWidget6);
        Inertia_Val->setObjectName(QStringLiteral("Inertia_Val"));
        Inertia_Val->setFont(font4);
        Inertia_Val->setDigitCount(3);

        gridLayout_7->addWidget(Inertia_Val, 1, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer, 11, 2, 1, 1);

        cd_ratio = new QLineEdit(layoutWidget6);
        cd_ratio->setObjectName(QStringLiteral("cd_ratio"));
        cd_ratio->setFont(font4);

        gridLayout_7->addWidget(cd_ratio, 7, 3, 1, 1);

        turnOff = new QPushButton(layoutWidget6);
        turnOff->setObjectName(QStringLiteral("turnOff"));
        turnOff->setFont(font4);

        gridLayout_7->addWidget(turnOff, 3, 0, 2, 2);

        VRControl = new QRadioButton(layoutWidget6);
        VRControl->setObjectName(QStringLiteral("VRControl"));
        sizePolicy2.setHeightForWidth(VRControl->sizePolicy().hasHeightForWidth());
        VRControl->setSizePolicy(sizePolicy2);
        VRControl->setFont(font4);

        gridLayout_7->addWidget(VRControl, 3, 2, 2, 1);

        Act2 = new QSlider(layoutWidget6);
        Act2->setObjectName(QStringLiteral("Act2"));
        Act2->setFont(font4);
        Act2->setMinimum(0);
        Act2->setMaximum(9);
        Act2->setSingleStep(1);
        Act2->setPageStep(10);
        Act2->setValue(0);
        Act2->setSliderPosition(0);
        Act2->setOrientation(Qt::Horizontal);
        Act2->setTickPosition(QSlider::NoTicks);

        gridLayout_7->addWidget(Act2, 13, 2, 1, 1);

        Mass_Val = new QLCDNumber(layoutWidget6);
        Mass_Val->setObjectName(QStringLiteral("Mass_Val"));
        Mass_Val->setFont(font4);
        Mass_Val->setDigitCount(5);

        gridLayout_7->addWidget(Mass_Val, 1, 3, 1, 1);

        reverseBox = new QRadioButton(layoutWidget6);
        reverseBox->setObjectName(QStringLiteral("reverseBox"));
        reverseBox->setFont(font4);

        gridLayout_7->addWidget(reverseBox, 7, 0, 1, 2);

        label_48 = new QLabel(layoutWidget6);
        label_48->setObjectName(QStringLiteral("label_48"));
        label_48->setFont(font4);
        label_48->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(label_48, 0, 0, 1, 2);

        CD_Val = new QLCDNumber(layoutWidget6);
        CD_Val->setObjectName(QStringLiteral("CD_Val"));
        CD_Val->setFont(font4);
        CD_Val->setDigitCount(3);

        gridLayout_7->addWidget(CD_Val, 1, 2, 1, 1);

        label_28 = new QLabel(layoutWidget6);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setFont(font4);
        label_28->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(label_28, 0, 3, 1, 1);

        inertiaValue = new QLineEdit(layoutWidget6);
        inertiaValue->setObjectName(QStringLiteral("inertiaValue"));
        inertiaValue->setFont(font4);

        gridLayout_7->addWidget(inertiaValue, 10, 3, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_2, 2, 2, 1, 1);

        equalBoxTouch = new QCheckBox(layoutWidget6);
        equalBoxTouch->setObjectName(QStringLiteral("equalBoxTouch"));
        sizePolicy2.setHeightForWidth(equalBoxTouch->sizePolicy().hasHeightForWidth());
        equalBoxTouch->setSizePolicy(sizePolicy2);
        equalBoxTouch->setFont(font4);

        gridLayout_7->addWidget(equalBoxTouch, 7, 2, 3, 1);

        Manual = new QPushButton(layoutWidget6);
        Manual->setObjectName(QStringLiteral("Manual"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Manual->sizePolicy().hasHeightForWidth());
        Manual->setSizePolicy(sizePolicy3);

        gridLayout_7->addWidget(Manual, 13, 3, 1, 1);

        showAllFramesCheckBox = new QCheckBox(centralWidget);
        showAllFramesCheckBox->setObjectName(QStringLiteral("showAllFramesCheckBox"));
        showAllFramesCheckBox->setGeometry(QRect(1780, 1140, 321, 71));
        showAllFramesCheckBox->setChecked(false);
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(100, 30, 701, 461));
        layoutWidget7 = new QWidget(groupBox_5);
        layoutWidget7->setObjectName(QStringLiteral("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(10, 30, 681, 421));
        gridLayout_12 = new QGridLayout(layoutWidget7);
        gridLayout_12->setSpacing(20);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        gridLayout_12->setContentsMargins(10, 10, 10, 10);
        FingerMappingExp = new QPushButton(layoutWidget7);
        FingerMappingExp->setObjectName(QStringLiteral("FingerMappingExp"));
        FingerMappingExp->setEnabled(true);
        sizePolicy3.setHeightForWidth(FingerMappingExp->sizePolicy().hasHeightForWidth());
        FingerMappingExp->setSizePolicy(sizePolicy3);

        gridLayout_12->addWidget(FingerMappingExp, 1, 0, 1, 1);

        dynamicEnvironment = new QPushButton(layoutWidget7);
        dynamicEnvironment->setObjectName(QStringLiteral("dynamicEnvironment"));
        sizePolicy3.setHeightForWidth(dynamicEnvironment->sizePolicy().hasHeightForWidth());
        dynamicEnvironment->setSizePolicy(sizePolicy3);

        gridLayout_12->addWidget(dynamicEnvironment, 0, 0, 1, 2);

        HoxelMappingExp = new QPushButton(layoutWidget7);
        HoxelMappingExp->setObjectName(QStringLiteral("HoxelMappingExp"));
        sizePolicy3.setHeightForWidth(HoxelMappingExp->sizePolicy().hasHeightForWidth());
        HoxelMappingExp->setSizePolicy(sizePolicy3);

        gridLayout_12->addWidget(HoxelMappingExp, 1, 1, 1, 1);

        MultiMassExp = new QPushButton(layoutWidget7);
        MultiMassExp->setObjectName(QStringLiteral("MultiMassExp"));
        sizePolicy3.setHeightForWidth(MultiMassExp->sizePolicy().hasHeightForWidth());
        MultiMassExp->setSizePolicy(sizePolicy3);

        gridLayout_12->addWidget(MultiMassExp, 2, 1, 1, 1);

        CubeGuidanceExp = new QPushButton(layoutWidget7);
        CubeGuidanceExp->setObjectName(QStringLiteral("CubeGuidanceExp"));
        CubeGuidanceExp->setEnabled(true);
        sizePolicy3.setHeightForWidth(CubeGuidanceExp->sizePolicy().hasHeightForWidth());
        CubeGuidanceExp->setSizePolicy(sizePolicy3);

        gridLayout_12->addWidget(CubeGuidanceExp, 2, 0, 1, 1);

        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(2160, 740, 351, 191));
        layoutWidget8 = new QWidget(groupBox_6);
        layoutWidget8->setObjectName(QStringLiteral("layoutWidget8"));
        layoutWidget8->setGeometry(QRect(10, 40, 331, 141));
        gridLayout = new QGridLayout(layoutWidget8);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(20);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        StiffnessExp = new QPushButton(layoutWidget8);
        StiffnessExp->setObjectName(QStringLiteral("StiffnessExp"));
        sizePolicy3.setHeightForWidth(StiffnessExp->sizePolicy().hasHeightForWidth());
        StiffnessExp->setSizePolicy(sizePolicy3);
        StiffnessExp->setFont(font4);

        gridLayout->addWidget(StiffnessExp, 0, 0, 1, 1);

        StiffnMassCombined = new QPushButton(layoutWidget8);
        StiffnMassCombined->setObjectName(QStringLiteral("StiffnMassCombined"));
        sizePolicy3.setHeightForWidth(StiffnMassCombined->sizePolicy().hasHeightForWidth());
        StiffnMassCombined->setSizePolicy(sizePolicy3);
        StiffnMassCombined->setFont(font4);

        gridLayout->addWidget(StiffnMassCombined, 1, 0, 1, 1);

        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(880, 10, 491, 451));
        layoutWidget9 = new QWidget(groupBox_7);
        layoutWidget9->setObjectName(QStringLiteral("layoutWidget9"));
        layoutWidget9->setGeometry(QRect(20, 44, 451, 281));
        gridLayout_10 = new QGridLayout(layoutWidget9);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        label_52 = new QLabel(layoutWidget9);
        label_52->setObjectName(QStringLiteral("label_52"));

        gridLayout_10->addWidget(label_52, 0, 0, 1, 1);

        ForX0 = new QLCDNumber(layoutWidget9);
        ForX0->setObjectName(QStringLiteral("ForX0"));
        ForX0->setDigitCount(6);

        gridLayout_10->addWidget(ForX0, 1, 0, 1, 1);

        ForY0 = new QLCDNumber(layoutWidget9);
        ForY0->setObjectName(QStringLiteral("ForY0"));
        ForY0->setDigitCount(6);

        gridLayout_10->addWidget(ForY0, 1, 1, 1, 1);

        ForZ0 = new QLCDNumber(layoutWidget9);
        ForZ0->setObjectName(QStringLiteral("ForZ0"));
        ForZ0->setDigitCount(6);

        gridLayout_10->addWidget(ForZ0, 1, 2, 1, 1);

        label_53 = new QLabel(layoutWidget9);
        label_53->setObjectName(QStringLiteral("label_53"));

        gridLayout_10->addWidget(label_53, 2, 0, 1, 1);

        ForX0_2 = new QLCDNumber(layoutWidget9);
        ForX0_2->setObjectName(QStringLiteral("ForX0_2"));
        ForX0_2->setDigitCount(6);

        gridLayout_10->addWidget(ForX0_2, 3, 0, 1, 1);

        ForY0_2 = new QLCDNumber(layoutWidget9);
        ForY0_2->setObjectName(QStringLiteral("ForY0_2"));
        ForY0_2->setDigitCount(6);

        gridLayout_10->addWidget(ForY0_2, 3, 1, 1, 1);

        ForZ0_2 = new QLCDNumber(layoutWidget9);
        ForZ0_2->setObjectName(QStringLiteral("ForZ0_2"));
        ForZ0_2->setDigitCount(6);

        gridLayout_10->addWidget(ForZ0_2, 3, 2, 1, 1);

        label_19 = new QLabel(layoutWidget9);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_10->addWidget(label_19, 4, 0, 1, 1);

        label_20 = new QLabel(layoutWidget9);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_10->addWidget(label_20, 4, 1, 1, 1);

        label_21 = new QLabel(layoutWidget9);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_10->addWidget(label_21, 4, 2, 1, 1);

        DesX0 = new QLCDNumber(layoutWidget9);
        DesX0->setObjectName(QStringLiteral("DesX0"));
        DesX0->setDigitCount(6);

        gridLayout_10->addWidget(DesX0, 5, 0, 1, 1);

        DesY0 = new QLCDNumber(layoutWidget9);
        DesY0->setObjectName(QStringLiteral("DesY0"));
        DesY0->setDigitCount(6);

        gridLayout_10->addWidget(DesY0, 5, 1, 1, 1);

        DesZ0 = new QLCDNumber(layoutWidget9);
        DesZ0->setObjectName(QStringLiteral("DesZ0"));
        DesZ0->setDigitCount(6);

        gridLayout_10->addWidget(DesZ0, 5, 2, 1, 1);

        layoutWidget10 = new QWidget(groupBox_7);
        layoutWidget10->setObjectName(QStringLiteral("layoutWidget10"));
        layoutWidget10->setGeometry(QRect(20, 340, 301, 101));
        gridLayout_16 = new QGridLayout(layoutWidget10);
        gridLayout_16->setSpacing(6);
        gridLayout_16->setContentsMargins(11, 11, 11, 11);
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        gridLayout_16->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget10);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_16->addWidget(label, 0, 0, 1, 1);

        label_26 = new QLabel(layoutWidget10);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_16->addWidget(label_26, 0, 1, 1, 1);

        MotorLCD_0 = new QLCDNumber(layoutWidget10);
        MotorLCD_0->setObjectName(QStringLiteral("MotorLCD_0"));
        MotorLCD_0->setDigitCount(6);

        gridLayout_16->addWidget(MotorLCD_0, 1, 0, 1, 1);

        StrokeLCD1_0 = new QLCDNumber(layoutWidget10);
        StrokeLCD1_0->setObjectName(QStringLiteral("StrokeLCD1_0"));
        StrokeLCD1_0->setDigitCount(6);

        gridLayout_16->addWidget(StrokeLCD1_0, 1, 1, 1, 1);

        groupBox_8 = new QGroupBox(centralWidget);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(1390, 10, 481, 451));
        layoutWidget11 = new QWidget(groupBox_8);
        layoutWidget11->setObjectName(QStringLiteral("layoutWidget11"));
        layoutWidget11->setGeometry(QRect(20, 40, 451, 281));
        gridLayout_14 = new QGridLayout(layoutWidget11);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        gridLayout_14->setContentsMargins(0, 0, 0, 0);
        label_57 = new QLabel(layoutWidget11);
        label_57->setObjectName(QStringLiteral("label_57"));

        gridLayout_14->addWidget(label_57, 0, 0, 1, 1);

        ForX1 = new QLCDNumber(layoutWidget11);
        ForX1->setObjectName(QStringLiteral("ForX1"));
        ForX1->setDigitCount(6);

        gridLayout_14->addWidget(ForX1, 1, 0, 1, 1);

        ForY1 = new QLCDNumber(layoutWidget11);
        ForY1->setObjectName(QStringLiteral("ForY1"));
        ForY1->setDigitCount(6);

        gridLayout_14->addWidget(ForY1, 1, 1, 1, 1);

        ForZ1 = new QLCDNumber(layoutWidget11);
        ForZ1->setObjectName(QStringLiteral("ForZ1"));
        ForZ1->setDigitCount(6);

        gridLayout_14->addWidget(ForZ1, 1, 2, 1, 1);

        label_58 = new QLabel(layoutWidget11);
        label_58->setObjectName(QStringLiteral("label_58"));

        gridLayout_14->addWidget(label_58, 2, 0, 1, 1);

        ForX1_2 = new QLCDNumber(layoutWidget11);
        ForX1_2->setObjectName(QStringLiteral("ForX1_2"));
        ForX1_2->setDigitCount(6);

        gridLayout_14->addWidget(ForX1_2, 3, 0, 1, 1);

        ForY1_2 = new QLCDNumber(layoutWidget11);
        ForY1_2->setObjectName(QStringLiteral("ForY1_2"));
        ForY1_2->setDigitCount(6);

        gridLayout_14->addWidget(ForY1_2, 3, 1, 1, 1);

        ForZ1_2 = new QLCDNumber(layoutWidget11);
        ForZ1_2->setObjectName(QStringLiteral("ForZ1_2"));
        ForZ1_2->setDigitCount(6);

        gridLayout_14->addWidget(ForZ1_2, 3, 2, 1, 1);

        label_64 = new QLabel(layoutWidget11);
        label_64->setObjectName(QStringLiteral("label_64"));

        gridLayout_14->addWidget(label_64, 4, 0, 1, 1);

        label_67 = new QLabel(layoutWidget11);
        label_67->setObjectName(QStringLiteral("label_67"));

        gridLayout_14->addWidget(label_67, 4, 1, 1, 1);

        label_69 = new QLabel(layoutWidget11);
        label_69->setObjectName(QStringLiteral("label_69"));

        gridLayout_14->addWidget(label_69, 4, 2, 1, 1);

        DesX1 = new QLCDNumber(layoutWidget11);
        DesX1->setObjectName(QStringLiteral("DesX1"));
        DesX1->setDigitCount(6);

        gridLayout_14->addWidget(DesX1, 5, 0, 1, 1);

        DesY1 = new QLCDNumber(layoutWidget11);
        DesY1->setObjectName(QStringLiteral("DesY1"));
        DesY1->setDigitCount(6);

        gridLayout_14->addWidget(DesY1, 5, 1, 1, 1);

        DesZ1 = new QLCDNumber(layoutWidget11);
        DesZ1->setObjectName(QStringLiteral("DesZ1"));
        DesZ1->setDigitCount(6);

        gridLayout_14->addWidget(DesZ1, 5, 2, 1, 1);

        layoutWidget12 = new QWidget(groupBox_8);
        layoutWidget12->setObjectName(QStringLiteral("layoutWidget12"));
        layoutWidget12->setGeometry(QRect(22, 338, 301, 101));
        gridLayout_15 = new QGridLayout(layoutWidget12);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        gridLayout_15->setContentsMargins(0, 0, 0, 0);
        label_59 = new QLabel(layoutWidget12);
        label_59->setObjectName(QStringLiteral("label_59"));
        label_59->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_15->addWidget(label_59, 0, 0, 1, 1);

        label_27 = new QLabel(layoutWidget12);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_15->addWidget(label_27, 0, 1, 1, 1);

        MotorLCD_1 = new QLCDNumber(layoutWidget12);
        MotorLCD_1->setObjectName(QStringLiteral("MotorLCD_1"));
        MotorLCD_1->setDigitCount(6);

        gridLayout_15->addWidget(MotorLCD_1, 1, 0, 1, 1);

        StrokeLCD1_1 = new QLCDNumber(layoutWidget12);
        StrokeLCD1_1->setObjectName(QStringLiteral("StrokeLCD1_1"));
        StrokeLCD1_1->setDigitCount(6);

        gridLayout_15->addWidget(StrokeLCD1_1, 1, 1, 1, 1);

        layoutWidget13 = new QWidget(centralWidget);
        layoutWidget13->setObjectName(QStringLiteral("layoutWidget13"));
        layoutWidget13->setGeometry(QRect(1232, 565, 411, 91));
        gridLayout_3 = new QGridLayout(layoutWidget13);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_36 = new QLabel(layoutWidget13);
        label_36->setObjectName(QStringLiteral("label_36"));
        label_36->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_36, 0, 0, 1, 1);

        label_37 = new QLabel(layoutWidget13);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_37, 0, 1, 1, 1);

        direction = new QLCDNumber(layoutWidget13);
        direction->setObjectName(QStringLiteral("direction"));
        direction->setDigitCount(6);

        gridLayout_3->addWidget(direction, 1, 0, 1, 1);

        condition = new QLCDNumber(layoutWidget13);
        condition->setObjectName(QStringLiteral("condition"));
        condition->setDigitCount(6);

        gridLayout_3->addWidget(condition, 1, 1, 1, 1);

        Condition1 = new QRadioButton(layoutWidget13);
        Condition1->setObjectName(QStringLiteral("Condition1"));

        gridLayout_3->addWidget(Condition1, 1, 2, 1, 1);

        groupBox_9 = new QGroupBox(centralWidget);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setEnabled(false);
        groupBox_9->setGeometry(QRect(110, 1220, 281, 81));
        layoutWidget14 = new QWidget(groupBox_9);
        layoutWidget14->setObjectName(QStringLiteral("layoutWidget14"));
        layoutWidget14->setGeometry(QRect(10, 30, 261, 41));
        gridLayout_8 = new QGridLayout(layoutWidget14);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        label_46 = new QLabel(layoutWidget14);
        label_46->setObjectName(QStringLiteral("label_46"));
        label_46->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_8->addWidget(label_46, 0, 0, 1, 1);

        alphaBox = new QDoubleSpinBox(layoutWidget14);
        alphaBox->setObjectName(QStringLiteral("alphaBox"));
        alphaBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        alphaBox->setMaximum(1);
        alphaBox->setSingleStep(0.01);
        alphaBox->setValue(1);

        gridLayout_8->addWidget(alphaBox, 0, 1, 1, 1);

        SetTrialNoButton = new QPushButton(centralWidget);
        SetTrialNoButton->setObjectName(QStringLiteral("SetTrialNoButton"));
        SetTrialNoButton->setGeometry(QRect(1230, 970, 381, 41));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 2540, 21));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Control Window", 0));
        label_16->setText(QApplication::translate("MainWindow", "Mass", 0));
        label_14->setText(QApplication::translate("MainWindow", "Box1", 0));
        label_15->setText(QApplication::translate("MainWindow", "Box2", 0));
        label_30->setText(QApplication::translate("MainWindow", "Box3", 0));
        label_17->setText(QApplication::translate("MainWindow", "Stiffness", 0));
        label_31->setText(QApplication::translate("MainWindow", "Graphics\n"
"Rate:", 0));
        label_22->setText(QApplication::translate("MainWindow", "Haptic\n"
"Rate:", 0));
        AdjustTrialNo->setText(QApplication::translate("MainWindow", "Adjust\n"
"Trial No.", 0));
        label_23->setText(QApplication::translate("MainWindow", "Trial No.", 0));
        label_24->setText(QApplication::translate("MainWindow", "Pair No.", 0));
        label_56->setText(QApplication::translate("MainWindow", "Starting Trial No.", 0));
        label_51->setText(QApplication::translate("MainWindow", "Subject No.", 0));
        planarCheckBox->setText(QApplication::translate("MainWindow", "Enable HME\n"
"Planar Constraint", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Control Method Toggle:", 0));
        forceControlButton->setText(QApplication::translate("MainWindow", "Force Control", 0));
        positionControlButton->setText(QApplication::translate("MainWindow", "Position Control", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Camera Control:", 0));
        label_65->setText(QApplication::translate("MainWindow", "Azimuth:", 0));
        resetCameraButton->setText(QApplication::translate("MainWindow", "Reset Camera", 0));
        label_62->setText(QApplication::translate("MainWindow", "Radius:", 0));
        label_63->setText(QApplication::translate("MainWindow", "Polar:", 0));
        lockCameraBox->setText(QApplication::translate("MainWindow", "Lock Camera", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "SERIAL", 0));
        label_41->setText(QApplication::translate("MainWindow", "BITNUM", 0));
        BitNumBox->setCurrentText(QApplication::translate("MainWindow", "8", 0));
        label_40->setText(QApplication::translate("MainWindow", "BAUD", 0));
        label_43->setText(QApplication::translate("MainWindow", "STOP BITS", 0));
        label_42->setText(QApplication::translate("MainWindow", "PARTIY", 0));
        BaudBox->setCurrentText(QApplication::translate("MainWindow", "115200", 0));
        label_39->setText(QApplication::translate("MainWindow", "PORT", 0));
        openButton->setText(QApplication::translate("MainWindow", "Serial\n"
"Not\n"
"Found", 0));
        label_44->setText(QApplication::translate("MainWindow", "X0 | Y0 | Z0", 0));
        label_45->setText(QApplication::translate("MainWindow", "X1 | Y1 | Z1", 0));
        label_49->setText(QApplication::translate("MainWindow", "Read:", 0));
        label_50->setText(QApplication::translate("MainWindow", "Write:", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Legacy:", 0));
        label_55->setText(QApplication::translate("MainWindow", "Act2", 0));
        label_60->setText(QApplication::translate("MainWindow", "CD", 0));
        label_11->setText(QApplication::translate("MainWindow", "Scaled Gravity", 0));
        label_32->setText(QApplication::translate("MainWindow", "CD Ratio", 0));
        equalBox->setText(QApplication::translate("MainWindow", "Equal", 0));
        JakeRenderCheckBox->setText(QApplication::translate("MainWindow", "JakeRender", 0));
        CD_Vertical_Box->setText(QApplication::translate("MainWindow", "CD Vertical", 0));
        label_35->setText(QApplication::translate("MainWindow", "Mass", 0));
        label_54->setText(QApplication::translate("MainWindow", "Act1", 0));
        turnOff->setText(QApplication::translate("MainWindow", "Turn Off", 0));
        VRControl->setText(QApplication::translate("MainWindow", "VR\n"
"Control", 0));
        reverseBox->setText(QApplication::translate("MainWindow", "Reverse", 0));
        label_48->setText(QApplication::translate("MainWindow", "Inertia", 0));
        label_28->setText(QApplication::translate("MainWindow", "Mass", 0));
        equalBoxTouch->setText(QApplication::translate("MainWindow", "Touch\n"
"Effective", 0));
        Manual->setText(QApplication::translate("MainWindow", "Manual", 0));
        showAllFramesCheckBox->setText(QApplication::translate("MainWindow", "Show All Frames", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Experiments:", 0));
        FingerMappingExp->setText(QApplication::translate("MainWindow", "Pick and Place\n"
"Experiment", 0));
        dynamicEnvironment->setText(QApplication::translate("MainWindow", "Dynamic\n"
"Bodies", 0));
        HoxelMappingExp->setText(QApplication::translate("MainWindow", "Wire Guide\n"
"Experiment", 0));
        MultiMassExp->setText(QApplication::translate("MainWindow", "Multi-Mass\n"
"Experiment", 0));
        CubeGuidanceExp->setText(QApplication::translate("MainWindow", "~TEST ENVIRONMENTS~\n"
"Experiment", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Mine's Experiments", 0));
        StiffnessExp->setText(QApplication::translate("MainWindow", "Stiffness\n"
"Experiment", 0));
        StiffnMassCombined->setText(QApplication::translate("MainWindow", "Stiffness-Mass\n"
"Experiment", 0));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "INDEX", 0));
        label_52->setText(QApplication::translate("MainWindow", "Stiffness Forces:", 0));
        label_53->setText(QApplication::translate("MainWindow", "Mass Forces:", 0));
        label_19->setText(QApplication::translate("MainWindow", "Des X", 0));
        label_20->setText(QApplication::translate("MainWindow", "Des Y", 0));
        label_21->setText(QApplication::translate("MainWindow", "Des Z", 0));
        label->setText(QApplication::translate("MainWindow", "Index Pot", 0));
        label_26->setText(QApplication::translate("MainWindow", "Desired Index", 0));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "THUMB", 0));
        label_57->setText(QApplication::translate("MainWindow", "Stiffness Forces:", 0));
        label_58->setText(QApplication::translate("MainWindow", "Mass Forces:", 0));
        label_64->setText(QApplication::translate("MainWindow", "Des X", 0));
        label_67->setText(QApplication::translate("MainWindow", "Des Y", 0));
        label_69->setText(QApplication::translate("MainWindow", "Des Z", 0));
        label_59->setText(QApplication::translate("MainWindow", "Thumb Pot", 0));
        label_27->setText(QApplication::translate("MainWindow", "Desired Thumb", 0));
        label_36->setText(QApplication::translate("MainWindow", "Direction:", 0));
        label_37->setText(QApplication::translate("MainWindow", "Condtion No.:", 0));
        Condition1->setText(QApplication::translate("MainWindow", "Cond1", 0));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "IIR Filter:", 0));
        label_46->setText(QApplication::translate("MainWindow", "Alpha = ", 0));
        SetTrialNoButton->setText(QApplication::translate("MainWindow", "Set New Trial Number", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
