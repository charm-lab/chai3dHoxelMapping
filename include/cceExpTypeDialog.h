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
//    explicit cceExpTypeDialog(int& cceExpType, QWidget* parent = nullptr);
    cceExpTypeDialog(int& cceExpType, int& mapping, QWidget* parent = nullptr);
    ~cceExpTypeDialog();
//    shared_data* p_CommonData;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:

    int& cceExpType;
    int& mapping;
    QLabel *textLabel;
    QPushButton *continueButton;


    QString getCCEExpTypePrompt(int expTypeVal, int mappingVal);

    void handleContinueButtonClick();
};

#endif // CCEEXPTYPEDIALOG_H
