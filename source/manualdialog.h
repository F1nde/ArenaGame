#ifndef MANUALDIALOG_H
#define MANUALDIALOG_H

#include <QDialog>

namespace Ui {
class ManualDialog;
}

class ManualDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManualDialog(QWidget *parent = 0);
    ~ManualDialog();

private slots:
    // Slot for closing the dialog
    void on_okButton_clicked();

private:
    Ui::ManualDialog *ui;
};

#endif // MANUALDIALOG_H
