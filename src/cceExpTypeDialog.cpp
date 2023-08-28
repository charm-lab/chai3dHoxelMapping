#include <QVBoxLayout>

#include "cceExpTypeDialog.h"

cceExpTypeDialog::cceExpTypeDialog(int& cceExpType, int& mapping, QWidget* parent) :
    QDialog(parent), cceExpType(cceExpType), mapping(mapping)
{
    // Adjust the font size as needed
    QFont font;
    font.setPointSize(20);

    // Create and set the text label
    //qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~cceExpTypeDialog.cpp Address of cceExpType: " << &(p_CommonData->cceExpType);
    textLabel = new QLabel(getCCEExpTypePrompt(cceExpType, mapping), this);
    //textLabel = new QLabel("Hi", this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setFont(font);

    continueButton = new QPushButton("Press ★ to Continue", this);
    continueButton->setEnabled(true);
    continueButton->setFixedSize(400, 50); // Adjust the width and height of the button as needed
    connect(continueButton, &QPushButton::clicked, this, &cceExpTypeDialog::handleContinueButtonClick);
    continueButton->setFont(font); // Set continue button font size

    // To recenter the button:
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(textLabel, 0, 0, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(continueButton, 1, 0, 1, 1, Qt::AlignCenter);

    setWindowTitle("Experiment Type Prompt");

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

QString cceExpTypeDialog::getCCEExpTypePrompt(int expTypeVal, int mappingVal)
{
    // p_CommonData->cceExpType
    QString message;

    qDebug()<< expTypeVal << " | " << mappingVal;

    if(expTypeVal == 0) // Shouldn't happen
    {
        qDebug()<< "Break Activated";
        message =  "Break Activated ( ~˘▾˘)~";
    }
    else if(expTypeVal == 1 && mappingVal == 1)
    {
        message =  "ʕっ• ᴥ • ʔっ\nTraining Trials with Feedback!:\nWhen force limit is exceeded\nColor changes.";
    }
    else if(expTypeVal == 1 && mappingVal == 5)
    {
        message =  "( ˘︹˘ )\nTraining Trials with NO Feedback:\nWhen force limit is exceeded\nColor changes.";
    }
    else if(expTypeVal == 2 && mappingVal == 1)
    {
        message =  "| (• ◡•)| (❍ᴥ❍ʋ)\nTesting Trials with Feedback!:\nWhen force limit is exceeded\nNo color change.";
    }
    else if(expTypeVal == 2 && mappingVal == 5)
    {
        message =  "༼つ ◕_◕ ༽つ\nTesting Trials with NO Feedback:\nWhen force limit is exceeded\nNo color change.";
    }
//    else if(val == 3)
//    {
//        message =  "(•ᵥᵥ•)\nExp Type 3 - When force limit is exceeded: \nNo color changes, trial ends. Try again next round.";
//    }
    else
    {
        message = "ERROR (┛ಠ_ಠ)┛彡┻━┻";
    }

    return message;
}
