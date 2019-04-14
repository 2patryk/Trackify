#ifndef DIALOG_EDIT_H
#define DIALOG_EDIT_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <tracking.h>
namespace Ui {
class Dialog_edit;
}

class Dialog_edit : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_edit(tracking trc,QSqlDatabase _baza_danych,QStringList firmykurierskie,QWidget *parent = nullptr);
    ~Dialog_edit();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog_edit *ui;
    QSqlDatabase baza_danych;
    int id;
};

#endif // DIALOG_EDIT_H
