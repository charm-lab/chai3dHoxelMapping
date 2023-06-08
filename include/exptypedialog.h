#ifndef EXPTYPEDIALOG_H
#define EXPTYPEDIALOG_H

#include <QDialog>
#include "mychai3dwindow.h"

class ExpTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExpTypeDialog(QWidget *parent = nullptr);
    ~ExpTypeDialog();


    int exec(); // Add this line

private:
    myCHAI3DWindow* m_parent;
};

#endif // EXPTYPEDIALOG_H
