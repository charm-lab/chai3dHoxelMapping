#include "mychai3dwindow.h"
#include "ui_mychai3dwindow.h"
#include "Widget_OpenGLDisplay.h"

myCHAI3DWindow::myCHAI3DWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myCHAI3DWindow)
{
    ui->setupUi(this);

#ifndef OCULUS
    //windowGLDisplay = new Widget_OpenGLDisplay(this->centralWidget());
    //Make the window that the environment appears the one that is editable in mainwindow.ui
    windowGLDisplay = new Widget_OpenGLDisplay(ui->openGLWidget);
    //Changes position and size of CHAI3D window:
    windowGLDisplay->setGeometry(QRect(0, 0, ui->openGLWidget->geometry().width(), ui->openGLWidget->geometry().height()));
    //windowGLDisplay->setGeometry(QRect(0, 0, 1700, 1200));
#endif

}

myCHAI3DWindow::~myCHAI3DWindow()
{
    delete ui;
}

void myCHAI3DWindow::Initialize()
{
#ifndef OCULUS
    windowGLDisplay->p_CommonData = p_CommonData;
#endif
}
