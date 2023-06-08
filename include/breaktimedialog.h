#ifndef BREAKTIMEDIALOG_H
#define BREAKTIMEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

class BreakTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BreakTimeDialog(QWidget *parent = nullptr);
    ~BreakTimeDialog();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLabel *remainingTimeLabel;
    QPushButton *continueButton;
    QTimer *countdownTimer;
    int remainingTimeInSeconds;

    void updateRemainingTime();
    void handleContinueButtonClick();
};


#endif // BREAKTIMEDIALOG_H
