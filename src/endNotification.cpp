#include <QVBoxLayout>
#include <QCoreApplication>
#include <QPushButton>

#include "endNotification.h"

EndNotification::EndNotification(QWidget *parent) : QDialog(parent)
{
    textLabel = new QLabel(this);
    textLabel->setText("Experiment Over! ʕ•́ᴥ•̀ʔっ\n\nThanks for Participating!~~");
    textLabel->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setPointSize(20);  // Adjust the font size as needed
    textLabel->setFont(font);

    endButton = new QPushButton("~Click to Close~", this);//new QPushButton("Press 'H' to Close", this);
    endButton->setEnabled(true);
    endButton->setFixedSize(400, 50); // Adjust the width and height of the button as needed
    endButton->setFont(font); // Set continue button font size

    //Make button cause app to close
    connect(endButton, &QPushButton::clicked, qApp, &QCoreApplication::quit, Qt::QueuedConnection);

    // To recenter the button:
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(textLabel, 0, 0, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(endButton, 1, 0, 1, 1, Qt::AlignCenter);

    setWindowTitle("Experiment Over! (>‿◠)✌");

    resize(800, 400);  // Adjust the width and height of the dialog as needed
}

EndNotification::~EndNotification()
{

}



