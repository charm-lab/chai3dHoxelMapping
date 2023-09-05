/********************************************************************************
** Form generated from reading UI file 'mychai3dwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYCHAI3DWINDOW_H
#define UI_MYCHAI3DWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_myCHAI3DWindow
{
public:
    QOpenGLWidget *openGLWidget;

    void setupUi(QWidget *myCHAI3DWindow)
    {
        if (myCHAI3DWindow->objectName().isEmpty())
            myCHAI3DWindow->setObjectName(QStringLiteral("myCHAI3DWindow"));
        myCHAI3DWindow->resize(2560, 1419);
        QIcon icon;
        icon.addFile(QStringLiteral("../Updated Finger Grasp.png"), QSize(), QIcon::Normal, QIcon::Off);
        myCHAI3DWindow->setWindowIcon(icon);
        openGLWidget = new QOpenGLWidget(myCHAI3DWindow);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setEnabled(false);
        openGLWidget->setGeometry(QRect(10, 10, 2540, 1420));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(12);
        font.setKerning(true);
        openGLWidget->setFont(font);

        retranslateUi(myCHAI3DWindow);

        QMetaObject::connectSlotsByName(myCHAI3DWindow);
    } // setupUi

    void retranslateUi(QWidget *myCHAI3DWindow)
    {
        myCHAI3DWindow->setWindowTitle(QApplication::translate("myCHAI3DWindow", "CHAI3D Haptics Environment", 0));
    } // retranslateUi

};

namespace Ui {
    class myCHAI3DWindow: public Ui_myCHAI3DWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYCHAI3DWINDOW_H
