#include <QVBoxLayout>

#include "trialNotification.h"

TrialNotification::TrialNotification(QWidget *parent) : QDialog(parent)
{
    textLabel = new QLabel(this);
    textLabel->setText("You Squeezed Too Hard (╥_╥)\n\nplz let go");
    textLabel->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setPointSize(20);  // Adjust the font size as needed
    textLabel->setFont(font);

    continueButton = new QPushButton("Press 'H' to Continue", this);
    continueButton->setEnabled(true);
    continueButton->setFixedSize(400, 50); // Adjust the width and height of the button as needed
    connect(continueButton, &QPushButton::clicked, this, &TrialNotification::handleContinueButtonClick);
    continueButton->setFont(font); // Set continue button font size

    // To recenter the button:
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(textLabel, 0, 0, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(continueButton, 1, 0, 1, 1, Qt::AlignCenter);

    setWindowTitle("Fail! D:");

    resize(800, 400);  // Adjust the width and height of the dialog as needed
}

TrialNotification::~TrialNotification()
{

}

void TrialNotification::handleContinueButtonClick()
{
    close();
}

void TrialNotification::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_H && continueButton->isEnabled())
    {
        handleContinueButtonClick(); // Click the "Continue" button
    }
    QDialog::keyPressEvent(event); // Allow default key press handling
}

