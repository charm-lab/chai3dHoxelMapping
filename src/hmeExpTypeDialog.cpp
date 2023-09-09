#include <QVBoxLayout>

#include "hmeExpTypeDialog.h"

hmeExpTypeDialog::hmeExpTypeDialog(int& hmeExpType, int& mapping, QWidget* parent) :
    QDialog(parent), hmeExpType(hmeExpType), mapping(mapping)
{
    qDebug()<<"start hmeExpTypeDialog";
    // Adjust the font size as needed
    QFont font;
    font.setPointSize(20);

    // Create and set the text label
    //qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hmeExpTypeDialog.cpp Address of hmeExpType: " << &(p_CommonData->hmeExpType);
    textLabel = new QLabel(gethmeExpTypePrompt(hmeExpType, mapping), this);
    //textLabel = new QLabel("Hi", this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setFont(font);

    continueButton = new QPushButton("Press ★ to Continue", this);
    continueButton->setEnabled(true);
    continueButton->setFixedSize(400, 50); // Adjust the width and height of the button as needed
    connect(continueButton, &QPushButton::clicked, this, &hmeExpTypeDialog::handleContinueButtonClick);
    continueButton->setFont(font); // Set continue button font size

    // To recenter the button:
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(textLabel, 0, 0, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(continueButton, 1, 0, 1, 1, Qt::AlignCenter);

    setWindowTitle("Experiment Type Prompt");

    resize(800, 400);  // Adjust the width and height of the dialog as needed
}

hmeExpTypeDialog::~hmeExpTypeDialog()
{

}

void hmeExpTypeDialog::handleContinueButtonClick()
{
    close();
}

void hmeExpTypeDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_D && continueButton->isEnabled())
    {
        handleContinueButtonClick(); // Click the "Continue" button
    }
    QDialog::keyPressEvent(event); // Allow default key press handling
}

QString hmeExpTypeDialog::gethmeExpTypePrompt(int expTypeVal, int mappingVal)
{
    // p_CommonData->hmeExpType
    QString message;

    qDebug()<< expTypeVal << " | " << mappingVal;

    if(expTypeVal == 0) // Shouldn't happen
    {
        qDebug()<< "Break Activated";
        message =  "Break Activated ( ~˘▾˘)~";
    }
    // Color change Notifications:
    else if(expTypeVal == 1 && mappingVal == 1)
    {
        message = "ʕっ• ᴥ • ʔっ\nTraining Trials!:\n"
                  "When cube starts to break,\n"
                  "it will turn red";
    }
    else if(expTypeVal == 1 && mappingVal == 3)
    {
        message = "¯\_( ͡❛ ͜ʖ ͡❛)_/¯\nTraining Trials!:\n"
                  "When cube starts to break,\n"
                  "it will turn red";
    }
    else if(expTypeVal == 1 && mappingVal == 5)
    {
        message = "( ˘︹˘ )\nTraining Trials:\n"
                  "When cube starts to break,\n"
                  "it will turn red";
    }
    // No color change Notifications:
    else if(expTypeVal == 2 && mappingVal == 1)
    {
        message = "| (• ◡•)| (❍ᴥ❍ʋ)\nTesting Trials!:\n"
                  "When force limit is exceeded,\n"
                  "no cube color change.";
    }
    else if(expTypeVal == 2 && mappingVal == 3)
    {
        message = "(ᴗᵔᴥᵔ)\nTesting Trials!:\n"
                  "When force limit is exceeded,\n"
                  "no cube color change.";
    }
    else if(expTypeVal == 2 && mappingVal == 5)
    {
        message = "༼つ ◕_◕ ༽つ\nTesting Trials!:\n"
                  "When force limit is exceeded,\n"
                  "no cube color change.";
    }
    else
    {
        message = "ERROR (┛ಠ_ಠ)┛彡┻━┻";
    }

    return message;
}
