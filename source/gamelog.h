#ifndef GAMELOG_H
#define GAMELOG_H

#include <QTextEdit>
#include <QString>

#define NEWLINE "<br>"

class GameLog : public QTextEdit
{
    Q_OBJECT

public:
    explicit GameLog(QWidget *parent);
    ~GameLog();

    // Inserts new log entry. The string is considered
    // as HTML which allows formatting.
    void insertHtml(QString);
};

#endif // GAMELOG_H
