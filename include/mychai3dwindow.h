#ifndef MYCHAI3DWINDOW_H
#define MYCHAI3DWINDOW_H

#include <QWidget>
#include "Widget_OpenGLDisplay.h"
#include "shared_data.h"


using namespace chai3d;
using namespace std;

namespace Ui {
class myCHAI3DWindow;
}

class myCHAI3DWindow : public QWidget
{
    Q_OBJECT

public:
    explicit myCHAI3DWindow(QWidget *parent = nullptr);
    ~myCHAI3DWindow();

     Widget_OpenGLDisplay* windowGLDisplay;
     shared_data* p_CommonData;

     // functions
     void Initialize();

private:
    Ui::myCHAI3DWindow *ui;
};

#endif // MYCHAI3DWINDOW_H
