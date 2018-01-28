#ifndef SCOREDIALOG_H
#define SCOREDIALOG_H

#include <QDialog>

namespace Ui {
class ScoreDialog;
}

class ScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScoreDialog(QWidget *parent = 0);
    ~ScoreDialog();

    // Set the score to be shown in the dialog
    void setScore(unsigned int score);

signals:
    void scoreScreen_newGame();
    void scoreScreen_exit();

private slots:
    // Slot for pressing the buttons
    void on_newGameButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::ScoreDialog *ui;
};

#endif // SCOREDIALOG_H
