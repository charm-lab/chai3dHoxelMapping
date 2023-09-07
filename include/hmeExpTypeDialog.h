#ifndef HMEEXPTYPEDIALOG_H
#define HMEEXPTYPEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>

#include "shared_data.h"

class hmeExpTypeDialog : public QDialog
{
    Q_OBJECT

public:
//    explicit hmeExpTypeDialog(int& hmeExpType, QWidget* parent = nullptr);
    hmeExpTypeDialog(int& hmeExpType, int& mapping, QWidget* parent = nullptr);
    ~hmeExpTypeDialog();
//    shared_data* p_CommonData;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:

    int& hmeExpType;
    int& mapping;
    QLabel *textLabel;
    QPushButton *continueButton;


    QString gethmeExpTypePrompt(int expTypeVal, int mappingVal);

    void handleContinueButtonClick();
};

#endif // HMEEXPTYPEDIALOG_H
