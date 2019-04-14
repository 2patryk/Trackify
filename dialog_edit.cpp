#include "dialog_edit.h"
#include "ui_dialog_edit.h"

#include <QDebug>

Dialog_edit::Dialog_edit(tracking trc,QSqlDatabase _baza_danych,QStringList firmykurierskie,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_edit)
{

    qDebug() << trc.nazwa << trc.numer;

    baza_danych = _baza_danych;
    id = trc.id;

    ui->setupUi(this);
    ui->cBkurier->addItems(firmykurierskie);
    ui->cBkurier->setCurrentText(trc.kurier);
    if(trc.nazwa != "not") ui->lenazwa->setText(trc.nazwa);
    ui->lenumer->setText(trc.numer);
setFixedSize(size());
}

Dialog_edit::~Dialog_edit()
{
    delete ui;
}

void Dialog_edit::on_buttonBox_accepted()
{
    QSqlQuery zapytanie(baza_danych);
    zapytanie.clear();

    tracking tmp;
    tmp.nazwa = ui->lenazwa->text();
    tmp.numer = ui->lenumer->text();
    if(tmp.nazwa == "") tmp.nazwa = "not";
    tmp.kurier = ui->cBkurier->currentText();

    zapytanie.exec("UPDATE TrackNum SET nazwa = '" + tmp.nazwa + "' , tracking = '" + tmp.numer + "', kurier = '"+ tmp.kurier   +"' WHERE id = " + QString::number(id));

    qDebug() << "UPDATE TrackNum SET nazwa = '" + tmp.nazwa + "' , tracking = '" + tmp.numer + "', kurier = "+ tmp.kurier  +" WHERE id = " + QString::number(id);

}
