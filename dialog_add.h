#ifndef DIALOG_ADD_H
#define DIALOG_ADD_H

#include "tracking.h"

#include <QDialog>
#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


namespace Ui {
class Dialog_add;
}

class Dialog_add : public QDialog
{
    Q_OBJECT

public:
    Dialog_add(QList<tracking> _numery, QSqlDatabase _baza_danych,QWidget *parent = nullptr);
    ~Dialog_add();
    QStringList getnumeryWyjscie();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog_add *ui;
    QList<tracking> numery;
    QSqlDatabase baza_danych;
    QMessageBox _msg;
    QStringList numeryWyjscie;
    int getLastID();

    QString checkCourier(QString num);
    void validateNumbers(QStringList tempNumeryLista);


};

#endif // DIALOG_ADD_H
