#include "dialog_add.h"
#include "tracking.h"
#include "ui_dialog_add.h"

#include <QtNetwork>



Dialog_add::Dialog_add(QList<tracking> _numery, QSqlDatabase _baza_danych,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add)
{
    numery = _numery;
    baza_danych = _baza_danych;
    ui->setupUi(this);
    setFixedSize(size());
}

Dialog_add::~Dialog_add()
{
    delete ui;
}

//void Dialog_add::validateNumbers(QStringList tempNumeryLista)
//{
//    //  Obsługa błędów
//    int POWTORZONY_NUMER_ERR = 2;

//    int kod_bledu = 0;
//    bool czy_powtorka = false;

//    QString powtorkiNumerow;


//    tempNumeryLista.removeDuplicates();
//    int lastID = getLastID();


//    for(int j=0; j<tempNumeryLista.count(); j++)
//    {
//        czy_powtorka = false;

//        if(tempNumeryLista[j].count() > 0)
//        {


//            qDebug() << tempNumeryLista[j].count();
//        for(int i=0; i<numery.count(); i++)
//        {
//                if(tempNumeryLista[j] == numery[i].numer)
//                {
//                    kod_bledu = POWTORZONY_NUMER_ERR;
//                    czy_powtorka = true;
//                    powtorkiNumerow+= tempNumeryLista[j] + ", ";
//                }
//        }



//        if(!czy_powtorka)
//        {
//            tracking tmp;
//            tmp.numer = tempNumeryLista[j];

//            if(baza_danych.isOpen())
//            {
//                QSqlQuery zapytanie(baza_danych);
//                QString data_godzina =  QDate::currentDate().toString("yyyy-MM-dd")+ " " + QTime::currentTime().toString("HH:mm:ss");
//                QString nazwa = "not";
//                QString kurier = checkCourier(tmp.numer);
//                bool czydostarczono = 0;

//                lastID++;
//                zapytanie.exec("INSERT INTO TrackNum (id, tracking, nazwa, kurier, czy_dostarczono, data_dodania) VALUES(" +
//                                QString::number(lastID)+", '" + tmp.numer + "', '"
//                                + nazwa + "', '" + kurier + "', '" + QString::number(czydostarczono) + "', '" + data_godzina + "')");
//                zapytanie.clear();

//                 qDebug() << "INSERT INTO TrackNum (id, tracking, nazwa, kurier, czy_dostarczono, data_dodania) VALUES(" +
//                             QString::number(lastID)+", '" + tmp.numer + "', '"
//                             + nazwa + "', '" + kurier + "', '" + QString::number(czydostarczono) + "', '" + data_godzina + "')";

//            }
//        }
//        }
//    }

//    if(kod_bledu==2)
//    {
//        powtorkiNumerow.remove(powtorkiNumerow.count()-2, 2);
//        _msg.setText("Nie dodano numerów: " + powtorkiNumerow + " z powodu powtórzenia");
//        _msg.exec();
//    }
//}

void Dialog_add::on_buttonBox_accepted()
{
    QString tempNumery = ui->ptNumeryPaczek->toPlainText();
    tempNumery.replace( " ", "" );
    tempNumery.replace( 9, "" );
    numeryWyjscie = tempNumery.split('\n');

}

//int Dialog_add::getLastID()
//{
//    if(numery.count() > 0)
//    {
//        return numery[numery.count()-1].id;
//    }
//    else
//    {
//        return 0;
//    }
//}

//QString Dialog_add::checkCourier(QString num)
//{
//    if(num.count() == 20) return "PP";
//    if(num.count() == 24) return "IP";
//    if(num.count() == 13)
//    {
//        qDebug() << num[0] << "  + " << num[1];
//    if((num[0] > 64 && num[0] < 123)  && (num[1] > 64 && num[1] < 123) && (num[11] > 64 && num[11] < 123) && (num[12] > 64 && num[12] < 123) )
//        return "PP";
//    }
//    return "NF";
//}

QStringList Dialog_add::getnumeryWyjscie()
{
    return numeryWyjscie;
}

