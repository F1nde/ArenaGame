#include "scoredialog.h"
#include "ui_scoredialog.h"

ScoreDialog::ScoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScoreDialog)
{
    ui->setupUi(this);
}

ScoreDialog::~ScoreDialog()
{
    delete ui;
}

void ScoreDialog::setScore(unsigned int score)
{
    ui->battlesLabel->setText(QString::number(score));
}

void ScoreDialog::on_newGameButton_clicked()
{
    emit scoreScreen_newGame();
    close();
}

void ScoreDialog::on_exitButton_clicked()
{
    emit scoreScreen_exit();
    close();
}
