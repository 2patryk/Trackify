#ifndef TRACKING_H
#define TRACKING_H


#include <QDate>
#include <QString>
#include "status.h"


class tracking
{
public:
    QString nazwa;
    tracking();
    int id;
    bool czydostarczono;

    QString numer;
    QDate data_nadania;
    QString rodzaj_przesylki;
    QDate data_dodania;
    QDateTime data_akt;
    QString kurier;
    QString krajn,krajd,miaston,miastod;
    QString aktualnystatus;
    QString extra;

    QList<status> lista_zdarzen;


    tracking & operator=(const tracking &);
    tracking (const tracking & tracking);
};

#endif // TRACKING_H
