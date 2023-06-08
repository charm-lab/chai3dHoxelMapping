#include <QVBoxLayout>
#include <QDialog>
#include <QKeyEvent>

#include "exptypedialog.h"
#include "mainwindow.h"
#include "mychai3dwindow.h"

ExpTypeDialog::ExpTypeDialog(QWidget *parent)
    : QDialog(parent)
{


    textLabel = new QLabel(this);
    textLabel->setText("This is a label");
    textLabel->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setPointSize(20);  // Adjust the font size as needed
    remainingTimeLabel->setFont(font);

    resize(800, 400);  // Adjust the width and height of the dialog as needed
}

ExpTypeDialog::~ExpTypeDialog()
{
    // Add any necessary cleanup code here
}

int ExpTypeDialog::exec()
{
    QDialog::exec(); // Call the base class exec() function
    // Add any additional logic if needed
}
