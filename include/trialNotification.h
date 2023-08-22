#ifndef TRIALNOTIFICATION_H
#define TRIALNOTIFICATION_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

class TrialNotification : public QDialog
{
    Q_OBJECT

public:
    //explicit TrialNotification(QWidget *parent = nullptr);
    TrialNotification(QString& notificationText, QWidget* parent = nullptr);
    ~TrialNotification();

signals:
    void hKeyPressed();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:

    //QString& notificationText;
    QString notificationText;

    QLabel *textLabel;
    QPushButton *continueButton;

    void handleContinueButtonClick();
};

#endif // TRIALNOTIFICATION_H
