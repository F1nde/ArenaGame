#include "gamelog.h"

GameLog::GameLog(QWidget *parent):
    QTextEdit(parent)
{
    // Make sure that the user can only read the log
    setReadOnly(true);
}

GameLog::~GameLog()
{
}

void GameLog::insertHtml(QString msg)
{
    // When appending a message, change line at the end
    QTextEdit::insertHtml(msg + NEWLINE);
    // Make sure that the new log shows on screen
    ensureCursorVisible();
}
