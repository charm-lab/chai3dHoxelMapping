#include <QVBoxLayout>
#include <QKeyEvent>

#include "breaktimedialog.h"

// Set break timer length:
double breakTimeLength = 5.0;//60.0;

BreakTimeDialog::BreakTimeDialog(QWidget *parent) : QDialog(parent)
{
    remainingTimeLabel = new QLabel(this);
    remainingTimeLabel->setText("Take a break!\n<~(￣︶￣)~>\nRemaining time: " +  QString::number(breakTimeLength) + " seconds");
    remainingTimeLabel->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setPointSize(20);  // Adjust the font size as needed
    remainingTimeLabel->setFont(font);

    continueButton = new QPushButton("Press 'H' to Continue", this);
    continueButton->setEnabled(false);
    continueButton->setFixedSize(400, 50); // Adjust the width and height of the button as needed
    connect(continueButton, &QPushButton::clicked, this, &BreakTimeDialog::handleContinueButtonClick);
    continueButton->setFont(font); // Set continue button font size

    // To recenter the button:
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(remainingTimeLabel, 0, 0, 1, 1, Qt::AlignCenter);
    mainLayout->addWidget(continueButton, 1, 0, 1, 1, Qt::AlignCenter);

    setWindowTitle("Break Time!");

    // Countdown Timer Stuff:
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &BreakTimeDialog::updateRemainingTime);

    remainingTimeInSeconds = breakTimeLength;
    countdownTimer->start(1000); // Start the countdown timer

    resize(800, 400);  // Adjust the width and height of the dialog as needed
}

BreakTimeDialog::~BreakTimeDialog()
{

}

void BreakTimeDialog::updateRemainingTime()
{
    remainingTimeInSeconds--;

    if (remainingTimeInSeconds <= 0)
    {
        countdownTimer->stop();
        continueButton->setEnabled(true); // Enable the continue button
    }

    QString timeText = QString::number(remainingTimeInSeconds) + " seconds";
    remainingTimeLabel->setText("Take a break!\n<~(￣︶￣)~>\nRemaining time: " + timeText);
}

void BreakTimeDialog::handleContinueButtonClick()
{
    close();
}

void BreakTimeDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_H && continueButton->isEnabled())
    {
        emit hKeyPressed();
        handleContinueButtonClick(); // Click the "Continue" button
    }
    QDialog::keyPressEvent(event); // Allow default key press handling
}

