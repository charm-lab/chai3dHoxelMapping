#ifndef TRIALFAILNOTIFICATION_H
#define TRIALFAILNOTIFICATION_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

class TrialFailNotification : public QDialog
{
    Q_OBJECT

public:
    explicit TrialFailNotification(QWidget *parent = nullptr);
    ~TrialFailNotification();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLabel *textLabel;
    QPushButton *continueButton;

    void handleContinueButtonClick();
};

#endif // TRIALFAILNOTIFICATION_H
