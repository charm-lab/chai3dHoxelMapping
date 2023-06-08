#ifndef CCEEXPTYPEDIALOG_H
#define CCEEXPTYPEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

#include "shared_data.h"

class cceExpTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cceExpTypeDialog(QWidget *parent = nullptr);
    ~cceExpTypeDialog();
    shared_data* p_CommonData;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLabel *textLabel;
    QPushButton *continueButton;

    QString getCCEExpTypePrompt(int expType);

    void handleContinueButtonClick();
};

#endif // CCEEXPTYPEDIALOG_H
