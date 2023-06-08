#include <QVBoxLayout>

#include "cceExpTypeDialog.h"
#include "mainwindow.h"
#include "mychai3dwindow.h"

int cceExpTypeVal = 0;

cceExpTypeDialog::cceExpTypeDialog(QWidget *parent) : QDialog(parent)
{
    // Adjust the font size as needed
    QFont font;
    font.setPointSize(20);

    // Create and set the text label
    cceExpTypeVal = p_CommonData->cceExpType;

    textLabel = new QLabel(getCCEExpTypePrompt(cceExpTypeVal), this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setFont(font);

    continueButton = new QPushButton("Press 'D' to Continue", this);
    continueButton->setEnabled(true);
    continueButton->setFixedSize(400, 50); // Adjust the width and height of the button as needed
    connect(continueButton, &QPushButton::clicked, this, &cceExpTypeDialog::handleContinueButtonClick);
    continueButton->setFont(font); // Set continue button font size

    // To recenter the button:
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(textLabel, 0, 0, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(continueButton, 1, 0, 1, 1, Qt::AlignCenter);

    setWindowTitle("Experiment Type Prompt");
    //setModal(true);

    resize(800, 400);  // Adjust the width and height of the dialog as needed
}

cceExpTypeDialog::~cceExpTypeDialog()
{

}

void cceExpTypeDialog::handleContinueButtonClick()
{
    close();
}

void cceExpTypeDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_D && continueButton->isEnabled())
    {
        handleContinueButtonClick(); // Click the "Continue" button
    }
    QDialog::keyPressEvent(event); // Allow default key press handling
}

QString cceExpTypeDialog::getCCEExpTypePrompt(int val)
{
    // p_CommonData->cceExpType
    qDebug() << "Address of cceExpType: " << &(p_CommonData->cceExpType);
    QString message;

    qDebug()<<val;

    if(val == 0)
    {
        qDebug()<< "Break Activated";
        message =  "Break Activated ( ~˘▾˘)~";
    }
    else if(val == 1)
    {
        message =  "ʕっ• ᴥ • ʔっ\nExp Type 1 - When force limit is exceeded: \nColor changes, trial continues.";
    }
    else if(val == 2)
    {
        message =  "| (• ◡•)| (❍ᴥ❍ʋ)\nExp Type 2 - When force limit is exceeded: \nNo color changes, trial continues.";
    }
    else if(val == 3)
    {
        message =  "(•ᵥᵥ•)\nExp Type 3 - When force limit is exceeded: \nNo color changes, trial ends. Try again next round.";
    }
    else
    {
        message = "ERROR (┛ಠ_ಠ)┛彡┻━┻";
    }

    return message;
}
