#ifndef STATUS_H
#define STATUS_H

#include <QDate>



class status
{
public:
    status();

    int id;
    QDateTime data;
    QString miejsce;
    QString zdarzenie;
    QString opis;
};

#endif // STATUS_H
