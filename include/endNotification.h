#ifndef ENDNOTIFICATION_H
#define ENDNOTIFICATION_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class EndNotification : public QDialog
{
    Q_OBJECT

public:
    explicit EndNotification(QWidget *parent = nullptr);
    ~EndNotification();


private:
    QLabel *textLabel;
    QPushButton *endButton;
};


#endif // ENDNOTIFICATION_H
